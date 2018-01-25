
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "../headers/MyDB_PageHandle.h"
#include "../headers/MyDB_BufferManager.h"

void *MyDB_PageHandleBase :: getBytes () {
	if (this->node->page->hasNotBeenEvicted) {
		this->node->page->bufferManager->updateLRU(this->node);
	} else {
        this->node->page->bufferManager->reloadVictim(this->node);
    }
    return this->node->page->address;
}

void MyDB_PageHandleBase :: wroteBytes () {
    this->node->page->isDirty = true;
    this->node->page->bufferManager->updateLRU(this->node);
}

MyDB_PageHandleBase :: MyDB_PageHandleBase (nodeptr node) {
    this->node = node;
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
    MyDB_pagetrl page = this->node->page;
    page->handleCount--;
    if (page->handleCount == 0) {
        if (page->isAnon) {
            this->node->page->bufferManager->addNewAddress(this->node);
            this->node->page->bufferManager->removeNode(this->node);
        } else {
            page->isPinned = false;
            // DO NOT DESTROY THE NODE OBEJECT!
        }
    }
}

nodeptr MyDB_PageHandleBase :: getNode(){
    return this->node;
}


#endif

