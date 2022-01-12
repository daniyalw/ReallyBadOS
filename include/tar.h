#pragma once

struct TarFile
{
    char *name;
    char typeflag;
    char linkname[100];
    u32 size;
    char * contents;

    char gid[8];
    char uid[8];
};

struct __attribute__((packed)) RawTar
{
    char name[100];     /*   0 */
    char mode[8];       /* 100 */
    char uid[8];        /* 108 */
    char gid[8];        /* 116 */
    char size[12];      /* 124 */
    char mtime[12];     /* 136 */
    char chksum[8];     /* 148 */
    char typeflag;      /* 156 */
    char linkname[100]; /* 157 */
    char magic[6];      /* 257 */
    char version[2];    /* 263 */
    char uname[32];     /* 265 */
    char gname[32];     /* 297 */
    char devmajor[8];   /* 329 */
    char devminor[8];   /* 337 */
    char prefix[155];   /* 345 */
    /* 500 */

    char __padding[12];

    u32 file_size()
    {
        u32 size = 0;
        u32 count = 1;

        for (u32 j = 11; j > 0; j--, count *= 8)
        {
            size += ((this->size[j - 1] - '0') * count);
        }

        return size;
    }
};

RawTar * headers[32];
int header_count = 0;

TarFile blocks[32];
int block_count = 0;

bool parse(unsigned int address);
