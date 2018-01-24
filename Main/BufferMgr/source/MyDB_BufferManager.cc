
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "../headers/MyDB_PageHandle.h"
#include "../../Catalog/headers/MyDB_Table.h"
#include "../headers/MyDB_BufferManager.h"
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr whichTable, long i) {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
	return nullptr;		
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
}

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
	this->pageSize = pageSize;
	this->numPages = numPages;
	this->tempFile = tempFile;
	this-> = 0;
	this->buffer = (char*) malloc(pageSize * numPages);
	this->head = new Node(NULL);
	this->tail = new Node(NULL);
    head->next = tail;
    tail->pre = head;
	for(int i = 0; i < numPages; i++){
		this->unUsedPages.push((this->buffer + i*pageSize));
	}
}

MyDB_BufferManager :: ~MyDB_BufferManager () {
}

void readData(string fileName, long offset, char* address){
    int fileHandle = open (fileName, O_CREAT|O_RDWR|O_SYNC, 0666);
    lseek(fileHandle, offset * this->pageSize,  SEEK_CUR);
    read(fileHandle, address, this->pageSize);
    close(fileHandle);
}

void insertNode(Node * cur){
    if (cur == NULL) return;
    Node* pre = this->tail->pre;
    pre->next = cur;
    cur->pre = pre;
    cur->next = this->tail;
    this->tail->pre = cur;
}

Node * removeNode(Node * cur){
    if (cur == NULL) return cur;
    Node* pre = cur->pre;
    Node* next = cur->next;
    pre->next = next;
    next->pre = pre;
    return cur;
}

Node * getVictim(){
    Node *cur = this->head->next;

    while (cur != this->tail && cur->isPinned) {
        cur = cur->next;
    }

    if (cur == this->tail) {
        return NULL;
    } else {
        return cur;
    }
}

void updateLRU(Node *cur){
    removeNode(cur);
    insertNode(cur);
}

// can be both types of page
void reloadVictim(MyDB_Page *victim){
    if (!this.unUsedPages.empty()) {

    } else {

    }
}

bool evict(){
    Node* victim = getVictim();

    if (victim == NULL) return false;

    removeNode(victim);

    MyDB_Page *page = victim->page;

    if (page->isDirty) {
        string fileName = page->isAnon ? page->tableOwner->getStorageLoc().c_str() : this->tempFile;

        int fileHandle = open (fileName, O_CREAT|O_RDWR|O_SYNC, 0666);
        lseek(fileHandle, page->offset * this->pageSize,  SEEK_CUR);
        write(fileHandle, page->address, this->pageSize);
    }

    page->hasNotBeenEvicted = true;

    if (page->handleCount > 0) {
        if(page->tableOwner != NULL) {
            pair<string, long> key(pageRef->tableOwner->getName(), page->offset);
            this->Map.erase(key);
        }
    } else {
        delete victim;
    }

    this->unUsedPages.push(page->address);



}
	
#endif


