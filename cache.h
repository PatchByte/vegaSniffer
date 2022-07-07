#ifndef _CACHE_HPP
#define _CACHE_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MapApi.h"
#include "packFile.h"

struct VegaCache
{
    void (*registerFiles)(struct VegaCache* ctx);
    struct PackFile* (*getFile)(struct VegaCache* ctx, const char* name);

    struct map* m;
};

struct VegaCache* createVegaCache();

#endif