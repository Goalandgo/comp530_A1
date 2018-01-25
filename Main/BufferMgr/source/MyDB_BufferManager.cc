
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "../headers/MyDB_PageHandle.h"
#include "../headers/MyDB_BufferManager.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
    pair<string,long> key(whichTable->getName(),i);
    auto it = this->Map.find(key);

    if (it == this->Map.end()) {
        if (!this->unUsedPages.empty()) {
            char * addr = this->unUsedPages.front();

            MyDB_pagetrl page = make_shared<MyDB_Page>(addr, whichTable, i, this, false, false);
            nodeptr cur = make_shared<Node>(page);

            this->readData(whichTable->getStorageLoc(),i,addr);

            this->insertNode(cur);
            this->Map.insert({key, cur});

            MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(cur);
            result->getNode()->page->handleCount++;
            return result;
        } else {
            evict();
            return getPage(whichTable, i);
        }
    } else {
        MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(it->second);

        updateLRU(it->second);

        result->getNode()->page->handleCount++;
        return result;
    }
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
    if(!this->unUsedPages.empty()){

        char * addr = this->unUsedPages.front();

        MyDB_pagetrl page = make_shared<MyDB_Page>(addr, nullptr, this->tempFileCurPosition++, this, false, true);
        nodeptr cur = make_shared<Node>(page);

        MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(cur);
        insertNode(cur);

        result->getNode()->page->handleCount++;
        return result;

    } else {
        evict();
        return getPage();
    }
}


MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr whichTable, long i) {
    pair<string,long> key(whichTable->getName(),i);
    auto it = this->Map.find(key);

    if (it == this->Map.end()) {
        if (!this->unUsedPages.empty()) {
            char * addr = this->unUsedPages.front();

            MyDB_pagetrl page = make_shared<MyDB_Page>(addr, whichTable, i, this, true, false);
            nodeptr cur = make_shared<Node>(page);

            this->readData(whichTable->getStorageLoc(),i,addr);

            this->insertNode(cur);
            this->Map.insert({key, cur});

            MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(cur);
            result->getNode()->page->handleCount++;
            return result;
        } else {
            evict();
            return getPage(whichTable, i);
        }
    } else {
        MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(it->second);

        updateLRU(it->second);

        result->getNode()->page->handleCount++;
        return result;
    }
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
    if(!this->unUsedPages.empty()){

        char * addr = this->unUsedPages.front();

        MyDB_pagetrl page = make_shared<MyDB_Page>(addr, nullptr, this->tempFileCurPosition++, this, true, true);
        nodeptr cur = make_shared<Node>(page);

        MyDB_PageHandle result = make_shared<MyDB_PageHandleBase>(cur);
        insertNode(cur);

        result->getNode()->page->handleCount++;
        return result;

    } else {
        evict();
        return getPage();
    }
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
    unpinMe->getNode()->page->isPinned = false;
}

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
	this->pageSize = pageSize;
	this->numPages = numPages;
	this->tempFile = tempFile;
	this->tempFileCurPosition = 0;
	this->buffer = (char*) malloc(pageSize * numPages);
	this->head = make_shared<Node>(nullptr);
	this->tail = make_shared<Node>(nullptr);
    head->next = tail;
    tail->pre = head;
	for(int i = 0; i < numPages; i++){
		this->unUsedPages.push((this->buffer + i*pageSize));
	}
}

MyDB_BufferManager :: ~MyDB_BufferManager () {
    nodeptr cur = this->head->next;
    while (cur != this->tail) {
        if (cur->page->isDirty) {
            string fileName = cur->page->isAnon ? this->tempFile : cur->page->tableOwner->getStorageLoc();
            writeData(fileName, cur->page->offset, cur->page->address);
        }
    }

    for ( auto it = filePool.begin(); it != filePool.end(); ++it ) //ERROR HERE
         close(it->second);
    delete buffer;
}

void MyDB_BufferManager :: addNewAddress(nodeptr cur){
    this->unUsedPages.push(cur->page->address);
}

void MyDB_BufferManager :: readData(string fileName, long offset, char* address){
    int fileHandle = 0;
    pair<string,long> key(fileName,offset);
    auto it = filePool.find(key);
    if (it == filePool.end()){
        fileHandle = open (fileName.c_str(), O_CREAT|O_RDWR|O_SYNC, 0666);
        filePool.insert({key, fileHandle});
    } else {
        fileHandle = it->second;
    }
    lseek(fileHandle, offset * this->pageSize,  SEEK_CUR);
    read(fileHandle, address, this->pageSize);
}

void MyDB_BufferManager :: writeData(string fileName, long offset, char* address){
    int fileHandle = 0;
    pair<string,long> key(fileName,offset);
    auto it = filePool.find(key);
    if (it == filePool.end()){
        fileHandle = open (fileName.c_str(), O_CREAT|O_RDWR|O_SYNC, 0666);
        filePool.insert({key, fileHandle});
    } else {
        fileHandle = it->second;
    }
    lseek(fileHandle, offset * this->pageSize,  SEEK_CUR);
    write(fileHandle, address, this->pageSize);
}
void MyDB_BufferManager :: insertNode(nodeptr cur){
    if (cur == NULL) return;
    nodeptr pre = this->tail->pre;
    pre->next = cur;
    cur->pre = pre;
    cur->next = this->tail;
    this->tail->pre = cur;
}

nodeptr MyDB_BufferManager :: removeNode(nodeptr cur){
    if (cur == NULL) return cur;
    nodeptr pre = cur->pre;
    nodeptr next = cur->next;
    pre->next = next;
    next->pre = pre;
    return cur;
}

nodeptr MyDB_BufferManager :: getVictim(){
    nodeptr cur = this->head->next;

    while (cur != this->tail && cur->page->isPinned) {
        cur = cur->next;
    }

    if (cur == this->tail) {
        return NULL;
    } else {
        return cur;
    }
}

void MyDB_BufferManager :: updateLRU(nodeptr cur){
    removeNode(cur);
    insertNode(cur);
}

// can be both types of page
void MyDB_BufferManager :: reloadVictim(nodeptr victim){
    if (!this->unUsedPages.empty()) {
        char *addr = this->unUsedPages.front();

        string fileName = victim->page->isAnon ? this->tempFile : victim->page->tableOwner->getStorageLoc();
        readData(fileName, victim->page->offset, addr);
        victim->page->address = addr;
    } else {
        this->evict();
        reloadVictim(victim);
    }
}

bool MyDB_BufferManager :: evict(){
    nodeptr victim = getVictim();

    if (victim == NULL) return false;

    removeNode(victim);

    MyDB_pagetrl page = victim->page;

    if (page->isDirty) {
        string fileName = page->isAnon ? page->tableOwner->getStorageLoc() : this->tempFile;
        writeData(fileName, page->offset, page->address);
    }

    page->hasNotBeenEvicted = false;

    if (page->handleCount == 0) {
        pair<string,long> key(page->tableOwner->getName(),page->offset);
        this->Map.erase(key);
        victim = NULL;
    }

    this->unUsedPages.push(page->address);
    return true;
}
	
#endif


