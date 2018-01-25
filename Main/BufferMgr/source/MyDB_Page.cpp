#include "../headers/MyDB_Page.h"//
#include "../../Catalog/headers/MyDB_Table.h"
// Created by 金建伟 on 2018/1/23.
//


MyDB_Page::MyDB_Page(char *address, const MyDB_TablePtr &tableOwner, long offset, MyDB_BufferManager *bufferManager,
                     bool isPinned, bool isAnon) : address(address), tableOwner(tableOwner), offset(offset),
                                                   bufferManager(bufferManager), isPinned(isPinned), isAnon(isAnon) {}

