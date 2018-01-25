//
// Created by 金建伟 on 2018/1/23.
//


#include "../headers/MyDB_Page.h"
#include "../headers/Node.h"

Node::Node(MyDB_pagetrl page){
    this->page = page;
}

Node::~Node(){
    this->page = nullptr;
}