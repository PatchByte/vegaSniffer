#include "ListApi.h"

#include <malloc.h>

struct vegaNode* newVegaNodeList()
{
    struct vegaNode* nde = malloc(sizeof(struct vegaNode));

    nde->key    = NULL;
    nde->value  = NULL;

    nde->prev   = NULL;
    nde->next   = NULL;

    return nde;
}

void freeVegaNodeList(struct vegaNode* list)
{
    struct vegaNode* cNode = getLastVegaNodeInList(list);

    do
    {
        struct vegaNode* cachedPtr = cNode->prev;
        
        free(cNode);

        cNode = cachedPtr;
    } while (cNode->prev != NULL);
}

struct vegaNode* getLastVegaNodeInList(struct vegaNode* nodeInList)
{
    struct vegaNode* cNode = nodeInList;

    do
    {
        if(cNode->next == NULL) break;
        cNode = cNode->next;
    } while (cNode != NULL);

    return cNode;
}

struct vegaNode* getFirstVegaNodeInList(struct vegaNode* nodeInList)
{
    struct vegaNode* cNode = nodeInList;

    do
    {
        if(cNode->prev == NULL) break;
        cNode = cNode->prev;
    } while (cNode != NULL);

    return cNode;
}

struct vegaNode* appendVegaNode(struct vegaNode* nodeInList)
{
    struct vegaNode* newNode = newVegaNodeList();

    struct vegaNode* lastNode = getLastVegaNodeInList(nodeInList);
    
    newNode->prev = lastNode;
    lastNode->next = newNode; 

    return newNode;
}

struct vegaNode* findVegaNodeWithKey(struct vegaNode* nodeInList, vegaKey sKey)
{
    struct vegaNode* cNode = getFirstVegaNodeInList(nodeInList);

    do
    {
        if(cNode->key == sKey)
        {
            return cNode;
        }

        cNode = cNode->next;
    } while (cNode->next != NULL);

    return NULL;
}