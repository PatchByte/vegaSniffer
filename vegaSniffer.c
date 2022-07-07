#include "vegaSniffer.h"
#include "netIncludes.h"

#include "vegaConfig.h"

#include <byteswap.h>

int VegaSniffer__CanSniff(struct VegaSniffer* snf)
{
    return snf->vegaConfig != 0;
}

void VegaSniffer_StartSniffing(struct VegaSniffer* snf)
{
    if(VegaSniffer__CanSniff(snf) == 0)
    {
        snf->lastActionFailed = 1;
        return;
    }

    snf->isListening = 1;

    struct ether_arp arpObj;

    while(snf->isListening == 1)
    {
        int len = recv(snf->sckFd, &arpObj, sizeof(struct ether_arp), 0);

        if(len == -1)
        {
            printf("[vega::sniffer] socket error recv!\n");
            exit(0);
        }

        if(len == 0) continue;

        unsigned int from_addr =
			(arpObj.arp_spa[3] << 24)
		      | (arpObj.arp_spa[2] << 16)
		      | (arpObj.arp_spa[1] << 8)
		      | (arpObj.arp_spa[0] << 0);

              from_addr = __swap32(from_addr);

        unsigned int target_addr =
                (arpObj.arp_tpa[3] << 24)
                | (arpObj.arp_tpa[2] << 16)
                | (arpObj.arp_tpa[1] << 8)
                | (arpObj.arp_tpa[0] << 0);

                target_addr = __swap32(target_addr);
        int newIps = 0;
        newIps += snf->vegaConfig->AddNode(snf->vegaConfig, from_addr);
        newIps += snf->vegaConfig->AddNode(snf->vegaConfig, target_addr);

        snf->stats.unqiueIps += newIps;
        snf->stats.capturedPackets++;
    }
}

struct VegaSniffer* CreateVegaSniffer()
{
    struct VegaSniffer* snf = malloc(sizeof(struct VegaSniffer));

    snf->stats.capturedPackets = 0;
    snf->stats.unqiueIps = 0;
    snf->isListening = 0;
    snf->vegaConfig = 0;
    snf->lastActionFailed = 0;

    snf->canStart = VegaSniffer__CanSniff;
    snf->startSniffing = VegaSniffer_StartSniffing;

    snf->sckFd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

    if(snf->sckFd <= 0)
    {
        printf("[vega::sniffer] error\n");
        exit(0);
    }



    return snf;
}
