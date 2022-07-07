#ifndef _PACKFILE_H
#define _PACKFILE_H

struct PackFile
{
    void (*Set)(struct PackFile* ctx, const char* type, const char* name, const char* route);
    void (*DecodeContent)(struct PackFile* ctx, const char* bEncoded);

    const char* m_type;
    const char* m_name;
    const char* m_content;
    const char* m_route;
};

struct PackFile* create_PackFile();

#endif