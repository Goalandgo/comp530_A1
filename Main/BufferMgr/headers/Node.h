//
// Created by 金建伟 on 2018/1/23.
//

#ifndef A1_NODE_H
#define A1_NODE_H

class Node;
#include "MyDB_Page.h"
using namespace std;

typedef shared_ptr <Node> nodeptr;

class Node{
        public:
            MyDB_pagetrl page;
            nodeptr pre;
            nodeptr next;

            Node(MyDB_pagetrl page);
            ~Node();

};
#endif //A1_NODE_H
