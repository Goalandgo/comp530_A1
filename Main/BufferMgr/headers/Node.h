//
// Created by 金建伟 on 2018/1/23.
//

#ifndef A1_NODE_H
#define A1_NODE_H

#include "MyDB_Page.h"
using namespace std;

class Node{
        public:
            MyDB_Page *page;
            Node *pre;
            Node *next;

            Node(MyDB_Page * page);
            ~Node();

};
#endif //A1_NODE_H
