#include "packFile.h"
#include <stdio.h>
#include <malloc.h>

static const char  table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int   BASE64_INPUT_SIZE = 57;

char isbase64(char c)
{
   return c && strchr(table, c) != 0;
}

char value(char c)
{
   const char *p = strchr(table, c);
   if(p) {
      return p-table;
   } else {
      return 0;
   }
}

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen)
{
   *dest = 0;
   if(*src == 0) 
   {
      return 0;
   }
   unsigned char *p = dest;
   do
   {

      char a = value(src[0]);
      char b = value(src[1]);
      char c = value(src[2]);
      char d = value(src[3]);
      *p++ = (a << 2) | (b >> 4);
      *p++ = (b << 4) | (c >> 2);
      *p++ = (c << 6) | d;
      if(!isbase64(src[1])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[2])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[3])) 
      {
         p--;
         break;
      }
      src += 4;
      while(*src && (*src == 13 || *src == 10)) src++;
   }
   while(srclen-= 4);
   *p = 0;
   return p-dest;
}


void PackFile__Set(struct PackFile* ctx, const char* type, const char* name, const char* route) {
    ctx->m_type    = type;
    ctx->m_name    = name;
    ctx->m_route   = route;
}

void PackFile__DecodeContent(struct PackFile* ctx, const char* bEncoded)
{
    size_t sRequired = strlen(bEncoded);
    ctx->m_content = (const char*)malloc(sRequired);
    memset((void*)ctx->m_content, 0, sRequired);

    UnBase64((unsigned char*)ctx->m_content, (const unsigned char*)bEncoded, sRequired);
}


struct PackFile* create_PackFile()
{
   struct PackFile* _pckFile = malloc(sizeof(struct PackFile));
   _pckFile->DecodeContent = &PackFile__DecodeContent;
   _pckFile->Set = &PackFile__Set;

   PackFile__Set(_pckFile, 0, 0, 0);

   return _pckFile;
}