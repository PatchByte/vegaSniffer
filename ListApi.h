#ifndef _LISTAPI_H
#define _LISTAPI_H

#include <stdio.h>

typedef uint32_t vegaKey;

struct vegaNode
{
    struct vegaNode* prev;
    struct vegaNode* next;

    vegaKey key;
    void* value;
};

struct vegaNode*    newVegaNodeList();
void                freeVegaNodeList(struct vegaNode* list);
struct vegaNode*    getLastVegaNodeInList(struct vegaNode* nodeInList);
struct vegaNode*    getFirstVegaNodeInList(struct vegaNode* nodeInList);
struct vegaNode*    appendVegaNode(struct vegaNode* nodeInList);

struct vegaNode*    findVegaNodeWithKey(struct vegaNode* nodeInList, vegaKey key);

#endif