#include "commonIncludes.h"
#include "netIncludes.h"

#define HTTPSERVER_IMPL
#include "httplib.h"

#include "cache.h"
#include "MapApi.h"

#include "vegaConfig.h"
#include "vegaSniffer.h"
#include "vegaTests.h"

#include "cJSON.h"

#include <pthread.h>
#include <linux/if_arp.h>

struct VegaCache* g_VegaCache;
struct VegaConfig* g_VegaConfig;
struct VegaSniffer* g_VegaSniffer;

int request_target_is(struct http_request_s* request, char const * target) {
  http_string_t url = http_request_target(request);
  int len = strlen(target);
  return len == url.len && memcmp(url.buf, target, url.len) == 0;
}

void handle_request(struct http_request_s* request) {
  struct http_response_s* response = http_response_init();
  http_response_status(response, 200);
  
  if(g_VegaConfig->NeedApiAuth(g_VegaConfig) == 1)
  {
    printf("%s\n", g_VegaConfig->apiKey);
    http_string_t apiKeyHeader = http_request_header(request, "vegaApiKey");

    if(apiKeyHeader.buf != 0 && apiKeyHeader.len > 0)
    {
      if(memcmp(apiKeyHeader.buf, g_VegaConfig->apiKey, strlen(g_VegaConfig->apiKey)) == 0)
      {

      }
      else
      {
        http_response_status(response, 451);
        http_respond(request, response);
        return;
      }
    }
    else
    {
      http_response_status(response, 451);
      http_respond(request, response);
      return;
    }
  }
  
  http_string_t trg = http_request_target(request);
  const char* reqUrl = trg.buf;
  const char* fmtUrl = (const char*)malloc(trg.len + 1);
  memset((void*)fmtUrl, 0, trg.len + 1);
  memcpy((void*)fmtUrl, (void*)reqUrl, trg.len);
  
  struct PackFile* targetFile = g_VegaCache->getFile(g_VegaCache, fmtUrl);

  if(g_VegaConfig->needsWebInterface != 1)
  {
    targetFile = NULL;
  }

  if(targetFile)
  {
    http_response_body(response, targetFile->m_content, strlen(targetFile->m_content));
    http_response_header(response, "Content-Type", targetFile->m_type);
  }
  else if(request_target_is(request, "/api/config"))
  {
    char* cnt = "{}";
    if(g_VegaSniffer->stats.unqiueIps > 0)
    {
      char* cnt = cJSON_Print(g_VegaConfig->jsonConfig);

      if(cnt)
      {
        http_response_body(response, cnt, strlen(cnt));
        http_response_header(response, "Content-Type", "application/json");

        cJSON_free(cnt);
      }
    }
    else
    {
      
      http_response_body(response, cnt, strlen(cnt));
      http_response_header(response, "Content-Type", "application/json");
    }
  }
  else if(request_target_is(request, "/api/setNodeState"))
  {
    http_string_t bdy = http_request_body(request);

    if(bdy.buf != 0 && bdy.len != 0)
    {
      char* bdyContent = malloc(bdy.len + 1);
      memset((void*)bdyContent, 0, bdy.len + 1);
      memcpy((void*)bdyContent, (const void*)bdy.buf, bdy.len);

      cJSON* reqObj = cJSON_Parse(bdyContent);

      if(reqObj != 0)
      {
        http_response_status(response, 204);

        cJSON* nodeStringItem = cJSON_GetObjectItem(reqObj, "node");
        cJSON* stateItem = cJSON_GetObjectItem(reqObj, "state");

        if(nodeStringItem != 0 && stateItem != 0)
        {
          char* nodeIp = cJSON_GetStringValue(nodeStringItem);
          if(nodeIp)
          {
            unsigned int ip = g_VegaConfig->IpToNode(nodeIp);
            if(ip > 0)
            {
              if(g_VegaConfig->HasNode(g_VegaConfig, ip) == 1)
              {
                cJSON_bool val = cJSON_IsTrue(stateItem);

                g_VegaConfig->SetNodeState(g_VegaConfig, ip, val);
              }
              else
              {
                http_response_status(response, 404);
              }
            }
            else
            {
              http_response_status(response, 411);
            }
          }
          else
          {
            http_response_status(response, 410);
          }
        }
        else
        {
          http_response_status(response, 410);
        }

        cJSON_Delete(reqObj);
      }
      else
      {
        http_response_status(response, 410);
      }

      free(bdyContent);
    }
  }
  else if(request_target_is(request, "/api/stats"))
  {
    cJSON* obj  = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "capturedPackets", g_VegaSniffer->stats.capturedPackets);
    cJSON_AddNumberToObject(obj, "unqiueIps", g_VegaSniffer->stats.unqiueIps);

    char* cnt = cJSON_Print(obj);

    http_response_body(response, cnt, strlen(cnt));
    http_response_header(response, "Content-Type", "text/json");

    cJSON_free(cnt);
  }
  else
  {
    printf("[vega::http] no resp %s\n", reqUrl);
  }

  if(fmtUrl) free((void*)fmtUrl);

  http_respond(request, response);
}

pthread_t snifferThread;

void* thread_sniffer(void* p)
{
  printf("[vega::sniffer] STARTING SNIFFING!\n");
  
  g_VegaSniffer->startSniffing(g_VegaSniffer);
  
  return p;
}


int main(int argc, char *argv[]) {
  g_VegaConfig = createVegaConfig();

  {
    // arg parsing
    size_t optind;
    for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
        switch (argv[optind][1]) {
        case 'h':
        {
          printf("Help:\n");
          printf("-h : Display's this\n");
          printf("-k : Set the Api Key\n");
          printf("-w : Start a Web Interface\n");
          printf("-t : Testing Mode!\n");
          exit(EXIT_SUCCESS);
          break;
        }
        case 'k':
        {
          g_VegaConfig->SetApiKey(g_VegaConfig, argv[optind+1]);
          optind = optind + 1;
          break;
        }
        case 'w':
        {
          g_VegaConfig->needsWebInterface = 1;
          break;
        }
        case 't':
        {
          g_VegaConfig->needsTesting = 1;
          break;
        }
        default:
            fprintf(stderr, "Usage: %s [-hkwt]\n", argv[0]);
            exit(EXIT_FAILURE);
        }   
    }
  }
  if(g_VegaConfig->needsTesting == 1) {
    printf("[vega::status] Running in testing mode!\n");
    runTests();
    exit(EXIT_SUCCESS);
  }
  {
    g_VegaCache = createVegaCache();
    g_VegaCache->registerFiles(g_VegaCache);
    g_VegaSniffer = CreateVegaSniffer();
    g_VegaSniffer->vegaConfig = g_VegaConfig;

    pthread_create(&snifferThread, NULL, thread_sniffer, NULL);
    
    printf("[vega::status] Running on port 0.0.0.0:%i\n", g_VegaConfig->webInterfacePort);

    struct http_server_s* server = http_server_init(g_VegaConfig->webInterfacePort, handle_request);
    http_server_listen(server);
  }
}