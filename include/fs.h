#pragma once

namespace Filesystem
{

namespace Ramdisk
{

typedef void (*__write)(char *);
typedef char * (*__read)();

typedef struct
{
    char * name;
    char * contents;
    char * path;
    char * parent;
    int id;
    uint32_t size;
    bool null = false;

    bool operator!() { return null; }

    __write write;
    __read read;
} FILE;

typedef struct
{
    char * name;
    char * path;
    FILE files[128];
    int file_count = 0;
    int id;
    uint32_t size;
    bool null = false;

    bool operator!() { return null; }
} FOLDER;

typedef struct
{
    char filename[128];
    char foldername[128];
} path_t;

FOLDER folders[128];
int folder_count = 0;

FILE null_file();
FOLDER null_folder();
FOLDER get_folder(char * name);
FILE get_file(char * name, char * folder);
FOLDER create_folder(char * name);
FILE create_file(char * name, char * folder, char * contents, uint32_t size);
FILE create_file(char * name, char * folder, char * (*_read)(), void (*_write)(char*));
FILE fopen(char * name, char * folder);

}

// for the disk
typedef struct
{
    char * name;
    char * file_names[128];
    int file_count = 0;
} disk_folder_t;

typedef struct
{
    disk_folder_t disk_folders[128];
    int disk_folder_count = 0;
} disk_header_t;

disk_header_t disk_header;

void ls();

}
