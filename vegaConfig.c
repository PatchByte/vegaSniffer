#include "vegaConfig.h"
#include "dnsLookup.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <byteswap.h>
#include <errno.h>
#include <math.h>

#include <string.h>

void vegaConfigError(const char* reason)
{
    printf("[vega::configError] %s", reason);
}

char* VegaNode__ToString(unsigned int val)
{
    char* buf = 0;
    size_t needed = snprintf(0, 0, "%d.%d.%d.%d",
     (val & 0xFF000000) >> 24,
     (val & 0x00FF0000) >> 16,
     (val & 0x0000FF00) >> 8,
     val & 0x000000FF);

    buf = malloc(needed + 2);
    memset((void*)buf, 0, needed + 2);
    snprintf(buf, needed + 1, "%d.%d.%d.%d",
     (val & 0xFF000000) >> 24,
     (val & 0x00FF0000) >> 16,
     (val & 0x0000FF00) >> 8,
     val & 0x000000FF);

    return buf;
}

unsigned int VegaNode__FromString(char* str)
{
    return __swap32(inet_addr(str));
}

int VegaConfig__HasNode(struct VegaConfig* cfg, unsigned int ip)
{
    char* fmtIp = VegaNode__ToString(ip);
    cJSON_bool ret = cJSON_HasObjectItem(cJSON_GetObjectItem(cfg->jsonConfig, "nodes"), fmtIp);

    free(fmtIp);

    return ret;
}

int VegaConfig__AddNode(struct VegaConfig* cfg, unsigned int ip)
{
    if(VegaConfig__HasNode(cfg, ip) > 0) return 0;

    char* fmtIp = VegaNode__ToString(ip);
    char* fmtHost = GetHostNameFromIp(fmtIp);

    cJSON* deviceObj = cJSON_CreateObject();
    cJSON_AddBoolToObject(deviceObj, "state", 0);
    cJSON_AddStringToObject(deviceObj, "hostname", fmtHost);
    cJSON_AddStringToObject(deviceObj, "ip", fmtIp);
    cJSON_AddItemToObject(cJSON_GetObjectItem(cfg->jsonConfig, "nodes"), fmtIp, deviceObj);

    free(fmtIp);
    free(fmtHost);

    return 1;
}

void VegaConfig__SetNodeState(struct VegaConfig* cfg, unsigned int ip, int state)
{

    char* fmtIp = VegaNode__ToString(ip);
    cJSON* deviceObj = cJSON_GetObjectItem(cJSON_GetObjectItem(cfg->jsonConfig, "nodes"), fmtIp);

    if(deviceObj)
    {
        cJSON_SetBoolValue(cJSON_GetObjectItem(deviceObj, "state"), state);
    }
    
    printf("[vega::config] Change -> %s:%s\n", fmtIp, state == 1 ? "true" : "false");
    
    free(fmtIp);
}

int VegaConfig__NeedApiAuth(struct VegaConfig* cfg)
{
    return cfg->apiKey != NULL;
}

void VegaConfig__SetApiKey(struct VegaConfig* cfg, char* key)
{
    if(cfg->apiKey != 0)
    {
        free(cfg->apiKey);
    }

    if(key == 0)
    {
        cfg->apiKey = NULL;
        return;
    }

    cfg->apiKey = strdup(key);
}

struct VegaConfig* createVegaConfig()
{
    struct VegaConfig* cfg = malloc(sizeof(struct VegaConfig));

    if(cfg == 0)
    {
        vegaConfigError( __FILE__ " : MALLOC CFG");
    }

    cfg->HasNode        = VegaConfig__HasNode;
    cfg->NodeToIp       = VegaNode__ToString;
    cfg->AddNode        = VegaConfig__AddNode;
    cfg->IpToNode       = VegaNode__FromString;
    cfg->SetNodeState   = VegaConfig__SetNodeState;

    cfg->NeedApiAuth    = VegaConfig__NeedApiAuth;
    cfg->SetApiKey      = VegaConfig__SetApiKey;

    cfg->jsonConfig = cJSON_CreateObject();

    srand(time(NULL));

    cfg->sessionId = (int) rand();
    cfg->apiKey = NULL;

    cfg->needsWebInterface = 0;
    cfg->webInterfacePort = 8090;

    cfg->needsTesting = 0;

    printf("[vega::sessionId] %04x\n", cfg->sessionId);

    if(cfg->jsonConfig == 0)
    {
        vegaConfigError( __FILE__ " : MALLOC CFG :: JSON");
    }

    cJSON* nodes = cJSON_CreateObject();

    cJSON_AddItemToObject(cfg->jsonConfig, "nodes", nodes);
    cJSON_AddNumberToObject(cfg->jsonConfig, "sessionId", cfg->sessionId);

    return cfg;
}