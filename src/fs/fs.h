#pragma once

#define P0 0
#define P1 1
#define P2 2
#define P3 3

struct File {
    char * name;
    char * data;
    int id;
    int permission;
};

struct Filesystem {
    File * files;
    int file_number;
};

Filesystem fs;

File open_file(char * fname);
File kopen_file(char * fname);
void write_file(char * fname, char * newdata);
void kwrite_file(char * fname, char * newdata);
void create_file(char * fname, char * fdata);
void kcreate_file(char * fname, char * fdata);
void krename_file(char * fname, char * newname);
void rename_file(char * fname, char * newname);
void kappend_file(char * fname, char * newdata);
void append_file(char * fname, char * newdata);
void log(char * data);
