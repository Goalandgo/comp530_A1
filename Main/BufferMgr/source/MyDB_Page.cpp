#include "../headers/MyDB_Page.h"//
#include "../../Catalog/headers/MyDB_Table.h"
// Created by 金建伟 on 2018/1/23.
//


MyDB_Page::MyDB_Page(char *address, const MyDB_TablePtr &tableOwner, long offset, MyDB_BufferManager *bufferManager,
                     bool isPinned, bool isAnon){
    this->address = address;
    this->tableOwner = tableOwner;
    this->offset = offset;
    this->bufferManager = bufferManager;
    this->isPinned = isPinned;
    this->isAnon = isAnon;
    this->isDirty = false;
    this->handleCount = 0;
    this->hasNotBeenEvicted = true;
}

MyDB_Page ::~MyDB_Page() {

}