#pragma once

namespace Filesystem
{

namespace VFS
{

typedef struct
{
    char * name;
    char * contents;
    char * path;
    char * parent;
    int id;
    bool null = false;
} FILE;

typedef struct
{
    char * name;
    char * path;
    FILE files[128];
    int file_count = 0;
    int id;
    bool null = false;
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
FILE create_file(char * name, char * folder, char * contents);

}

void ls();

}
