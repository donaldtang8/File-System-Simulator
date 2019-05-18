#ifndef _DNODE_HPP_
#define _DNODE_HPP_

#include <cstdio>

struct DNode{
    DNode(int s, int l, int u){
        start=s;
        end=l;
        next=NULL;
        usedBit=u;
    }
    int usedBit;  
    int start;
    int end;
    DNode* next; 
};

#endif
