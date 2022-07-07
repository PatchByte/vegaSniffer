#ifndef _VEGASNIFFER_H
#define _VEGASNIFFER_H

struct VegaSnifferStats
{
    unsigned int capturedPackets;
    unsigned int unqiueIps;
};

struct VegaSniffer
{
    int isListening;
    int lastActionFailed;
    struct VegaConfig* vegaConfig;
    struct VegaSnifferStats stats;

    int sckFd;

    int (*canStart)(struct VegaSniffer* snf);
    void (*startSniffing)(struct VegaSniffer* snf);
};

struct VegaSniffer* CreateVegaSniffer();

#endif