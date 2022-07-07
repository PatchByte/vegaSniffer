#include "dnsLookup.h"

#include <stdio.h> //for printf()
#include <stdlib.h> //for exit()
#include <arpa/inet.h> //for inet_pton()
#include <netdb.h> // for NI_MAXHOST, getnameinfo() and gai_strerror()
#include <errno.h> // for errno
#include <string.h> // for strerror()

char* GetHostNameFromIp(char* ip)
{
     struct sockaddr_in sa;
  int res = inet_pton(AF_INET, ip , &sa.sin_addr);
  switch(res) {
    case 0: printf("\nInput address is not a valid IPv4 address.\n");
    case -1: if(res == -1)
               printf("\nError(%s)\n",strerror(errno));
             int n_res = inet_pton(AF_INET6, ip , &sa.sin_addr);
             switch(n_res) {
               case 0: printf("\nInput address is not a valid IPv6 address.\n");
               case -1: if(n_res == -1)
                          printf("\nError(%s)\n",strerror(errno));
                        exit(-1);
               case 1: sa.sin_family = AF_INET6;
             }
    case 1: sa.sin_family = AF_INET;
  }

  char node[NI_MAXHOST];
  memset(node,0,NI_MAXHOST);
  res = getnameinfo((struct sockaddr*)&sa, sizeof(sa), node, sizeof(node), NULL, 0, 0);
  if (res) {
    printf("%s\n", gai_strerror(res));
    return NULL;
  }

  return strdup(node);

}