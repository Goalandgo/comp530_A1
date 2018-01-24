//
// Created by 金建伟 on 2018/1/23.
//

#ifndef A1_MYDB_PAGE_H
#define A1_MYDB_PAGE_H

#include <memory>
#include <string>

#include "../../Catalog/headers/MyDB_Table.h"

class MyDB_BufferManager;

using namespace std;

class MyDB_Page{

public:
    MyDB_Page(char* address,  MyDB_TablePtr tableOwner, long index, MyDB_BufferManager * bufferManager, bool isPinned, bool isAnon);

    MyDB_Page(char *address);

    MyDB_Page(char *address, const MyDB_TablePtr &tableOwner, long offset, MyDB_BufferManager *bufferManager,
              bool isPinned, bool isAnon);

    // memory address to put page content
    char* address;

    // the table that this page belongs to
    MyDB_TablePtr tableOwner;

    // the position that this page located in this table
    long offset;

    // buffer Manager that owns this page
    // update LinkedList, update available buffer
    MyDB_BufferManager *  bufferManager;

    // if this page has been pinned
    bool isPinned;

    // if this page is an anonymous page
    bool isAnon;

    // the number of handles that point to this page
    int handleCount;

    // if this page has been evicted before, then the address is not available
    bool hasNotBeenEvicted;

    // if this page has been changed in memory and has not been written back into SSD.
    bool isDirty;

    ~MyDB_Page();

};


#endif //A1_MYDB_PAGE_H
