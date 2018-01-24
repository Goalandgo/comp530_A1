//
// Created by 金建伟 on 2018/1/23.
//

#include "Node.h"

Node::Node(MyDB_Page * page){
    this->page = page;
}

Node::~Node(){
    delete this->page;
}