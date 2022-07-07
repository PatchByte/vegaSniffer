from functools import cache
import os
import base64

targetFiles = [
    {
        "name": "sniffer.html",
        "path": "web/sniffer.html",
        "route": "/",
        "type": "text/html"
    },
    {
        "name": "toggle.css",
        "path": "web/toggle.css",
        "route": "/toggle.css",
        "type": "text/css"
    }
]

# real code do not modify please

cacheFileBegin ="""// GENERATED FILE START
#include "cache.h"
#include "MapApi.h"

void VegaCache_registerFiles(struct VegaCache* ctx)
{
    ctx->m = mapNew();
"""

cacheFileEnd = """}

struct PackFile* VegaCache_getFile(struct VegaCache* ctx, const char* name)
{
    return (struct PackFile*)mapGet((char*)name, ctx->m);
}

struct VegaCache* createVegaCache()
{
    struct VegaCache* _vgaCache = malloc(sizeof(struct VegaCache));

    _vgaCache->registerFiles = &VegaCache_registerFiles;
    _vgaCache->getFile =  &VegaCache_getFile;

    return _vgaCache;
}
// GENERATED FILE END
"""

cacheFileOuput = cacheFileBegin

for x in targetFiles:
    yFile = open(x["path"], "r")
    if yFile:
        cnt = yFile.read()
        cnt = base64.b64encode(cnt.encode()).decode("utf-8")
        cacheFileOuput += "\t{ struct PackFile* _pck = create_PackFile(); _pck->DecodeContent(_pck, \"%s\"); _pck->Set(_pck, \"%s\", \"%s\", \"%s\"); mapAdd(_pck->m_route, (void*)_pck, ctx->m); }" % (cnt, x["type"], x["name"], x["route"]) 
        cacheFileOuput += "\n"

cacheFileOuput += cacheFileEnd
open("cache.c", "w+").write(cacheFileOuput)