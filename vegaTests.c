
#include "vegaTests.h"
#include "ListApi.h"

void runTests()
{
    struct vegaNode* beginNode = newVegaNodeList();

    beginNode->key = 1;
    beginNode->value = 1;

    struct vegaNode* next1 = appendVegaNode(beginNode);

    next1->key = 2;
    next1->value = 2;

    struct vegaNode* next2 = appendVegaNode(beginNode);

    next2->key = 3;
    next2->value = 3;

    struct vegaNode* cNode = beginNode;

    do
    {
        printf("%p:%04x:%04x\n", cNode, cNode->key, cNode->value);

        cNode = cNode->next;
    } while (cNode != NULL);
    
    printf("FK: FIRST: %p, LAST: %p\n", getFirstVegaNodeInList(next2), getLastVegaNodeInList(beginNode));
    printf("RL: FIRST: %p, LAST: %p\n", beginNode, next2);
    printf("CT: %i\n", getCountOfVegaNodes(getFirstVegaNodeInList(next1)));

    freeVegaNodeList(next2);
}