cmake_minimum_required(VERSION 3.9)
project(A1)

set(CMAKE_CXX_STANDARD 11)

add_executable(A1
        Build/SConstruct
        Main/BufferMgr/headers/MyDB_BufferManager.h
        Main/BufferMgr/headers/MyDB_Page.h
        Main/BufferMgr/headers/MyDB_PageHandle.h
        Main/BufferMgr/headers/Node.h
        Main/BufferMgr/source/MyDB_BufferManager.cc
        Main/BufferMgr/source/MyDB_Page.cpp
        Main/BufferMgr/source/MyDB_PageHandle.cc
        Main/BufferMgr/source/Node.cpp
        Main/BufferTest/source/BufferQUnit.cc
        Main/Catalog/headers/MyDB_Table.h
        Main/Catalog/source/MyDB_Table.cc
        Main/Qunit/headers/QUnit.h)
