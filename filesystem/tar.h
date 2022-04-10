#pragma once

#define TAR_FILE '0'
#define TAR_HARD_LINK '1'
#define TAR_SYMBOLIC_DEVICE '2'
#define TAR_CHARACTER_DEVICE '3'
#define TAR_BLOCK_DEVICE '4'
#define TAR_DIR '5'
#define TAR_NAMED_PIPE '6'

struct TarDesc {
    char name[30];
    char type;
};

TarDesc tar_types[] = {
    {"file", '0'},
    {"hard link", '1'},
    {"symbolic device", '2'},
    {"character device", '3'},
    {"block device", '4'},
    {"directory", '5'},
    {"named pipe", '6'},
    {NULL, NULL}
};

char *tar_find_type(char type) {
    int z = 0;

    while (tar_types[z].name != NULL) {
        if (tar_types[z].type == type) {
            return tar_types[z].name;
        }
    }

    return NULL;
}

struct TarFile
{
    char *name;
    char typeflag;
    char linkname[100];
    u32 size;
    char * contents;

    char gid[8];
    char uid[8];

    char type;
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

struct Tar
{
public:
    RawTar * headers[32];
    int header_count = 0;

    TarFile blocks[32];
    int block_count = 0;

    bool parse(unsigned int address);
};
