#pragma once

#define R0 0
#define R1 1
#define R2 2
#define R3 3

typedef struct {
    char * name;
    char * contents;
    int id;
    int permission_level;
} file_t;

file_t * files[128];
int file_count = 0;

void fs_init();
file_t * create_file(char * name, char * data);
file_t * kcreate_file(char * name, char * data);
void rename_file(char * name);
void kdelete_file(char * name);
void krename_file(char * name);
file_t * kopen_file(char * name);
void save_file(file_t * file);
file_t * get_file_from_name(char * name);
void delete_file(char * name);
