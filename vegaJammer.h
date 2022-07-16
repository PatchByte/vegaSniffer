#ifndef _VEGAJAMMER_H
#define _VEGAJAMMER_H

struct VegaJammer
{
    struct vegaNode* threadList;
};

struct VegaJammer* createVegaJammer();

#endif