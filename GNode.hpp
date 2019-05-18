#ifndef _GNode_HPP_
#define _GNode_HPP_

#include <list>
#include "LFile.hpp"

struct GNode{
    GNode(std::string nm,int ts, int sz, GNode* pt){
        name=nm;
        size=sz;
        parent=pt;
        file = NULL;
        timeStamp=ts;
    }

    ~GNode()
    {
        if(file != NULL)
        {
            delete file;
        }
    }

    LFile* file;
    std::string name; 
    int timeStamp;
    int size;
    std::list <GNode*> children;
    GNode* parent;
};

#endif
