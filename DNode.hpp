// Donald Tang and Tianwei Liu
// dtang4
// I pledge my honor I have abided by Stevens Honor System.

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
