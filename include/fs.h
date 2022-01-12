#pragma once

#include <tar.h>

namespace Filesystem
{

namespace VFS
{

struct FILE
{
    char * name;
    char gid[8];
    char uid[8];
    uint32_t size;
    char * contents;
    bool null = false;

    bool operator!() { return null; }
};

std::list<FILE> all_files;

void write(char * name, char * data);
void close(char *name);
FILE fopen(char * name);
char *read(char * fname);
int get_file_z(char * name);
FILE get_file_n(char * name);
void list_dir();
void save_file(FILE f);
FILE fcreate(TarFile block);
FILE f_null_file();

}

namespace Ramdisk
{

typedef struct
{
    char * name;
    char * contents;
    char * path;
    char * parent;
    int id;
    bool null = false;

    bool operator!() { return null; }
} RFILE;

typedef struct
{
    char * name;
    char * path;
    RFILE files[128];
    int file_count = 0;
    int id;
    bool null = false;

    bool operator!() { return null; }
} RFOLDER;

typedef struct
{
    char filename[128];
    char foldername[128];
} path_t;

RFOLDER folders[128];
int folder_count = 0;

RFILE null_file();
RFOLDER null_folder();
RFOLDER get_folder(char * name);
RFILE get_file(char * name, char * RFOLDER);
RFOLDER create_folder(char * name);
RFILE create_file(char * name, char * RFOLDER, char * contents);

}

void ls();

}
