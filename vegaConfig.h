#ifndef _VEGACONFIG_H
#define _VEGACONFIG_H

#include "cJSON.h"

struct VegaConfig
{
    cJSON* jsonConfig;

    int sessionId;
    char* apiKey;
    
    int needsTesting;
    
    int needsWebInterface;
    int webInterfacePort;

    int                 (*AddNode)      (struct VegaConfig* cfg, unsigned int ip);
    int                 (*HasNode)      (struct VegaConfig* cfg, unsigned int ip);
    void                (*SetNodeState) (struct VegaConfig* cfg, unsigned int ip, int state);
    char*               (*NodeToIp)     (unsigned int ip);
    unsigned int        (*IpToNode )    (char* ip);
    int                 (*NeedApiAuth)  (struct VegaConfig* cfg);
    void                (*SetApiKey)    (struct VegaConfig* cfg, char* apiKey);
};

struct VegaConfig* createVegaConfig();

#endif