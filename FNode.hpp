#ifndef _FNode_HPP_
#define _FNode_HPP_
#include <cstdio>

struct FNode{
    int blockAddress;
    FNode* next;    

    FNode()
    {
        blockAddress = -1;
        next = NULL;
    }

    FNode(int m, FNode* p, FNode* n)
    {
        blockAddress = m;
        next = n;
    }
};


#endif
