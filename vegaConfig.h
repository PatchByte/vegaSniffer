#ifndef _VEGACONFIG_H
#define _VEGACONFIG_H

#include "cJSON.h"

struct VegaConfig
{
    cJSON* jsonConfig;
    int sessionId;

    int                 (*AddNode)      (struct VegaConfig* cfg, unsigned int ip);
    int                 (*HasNode)      (struct VegaConfig* cfg, unsigned int ip);
    void                (*SetNodeState) (struct VegaConfig* cfg, unsigned int ip, int state);
    char*               (*NodeToIp)     (unsigned int ip);
    unsigned int        (*IpToNode )    (char* ip);
};

struct VegaConfig* createVegaConfig();

#endif