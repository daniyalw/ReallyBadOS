#pragma once

#include "env.h"

char *getenv(char *name) {
    char *path = (char *)malloc(100);
    memset(path, 0, 100);
    get(path, "/env/%s", name);

    FILE *file = fopen(path, "r");

    if (!file) {
        return NULL;
    }

    int size = fsize(file);

    char *buffer = (char *)malloc(size + 1);
    memset(buffer, 0, size + 1);
    fread(file, 1, size, buffer);

    free(path);
    fclose(file);

    return buffer;
}

int setenv(char *name, char *val, int overwrite) {
    if (overwrite) {
        char *path = (char *)malloc(100);
        memset(path, 0, 100);
        get(path, "/env/%s", name);

        FILE *file = fopen(path, "w");

        if (!file) {
            return NULL;
        }

        int ret = fwrite(file, 0, strlen(val), val);

        free(path);
        fclose(file);

        return ret;
    }

    return 0;
}

int env_read(fs_node_t *node, int offset, int length, char *buffer) {
    for (int z = 0; z < env_count; z++) {
        if (strcmp(env_vec[z].name, node->name) == 0) {
            strcpy(buffer, env_vec[z].val);
            return 0;
        }
     }

     buffer = NULL;
     return 1;
}

int env_write(fs_node_t *node, int offset, int length, char *buffer) {
    for (int z = 0; z < env_count; z++) {
        if (strcmp(env_vec[z].name, node->name) == 0) {
            auto _map = env_vec[z];
            _map.val = buffer;
            env_vec[z] = _map;

            return 0;
        }
     }

     return 1;
}

int env_mkdir(fs_node_t *node) {
    return 1; // stop vfs from actually creating folder
}

int env_mkfile(fs_node_t *node) {
    for (int z = 0; z < env_count; z++) {
        if (strcmp(env_vec[z].name, node->name) == 0) {
            return 1; // file already exists
        }
     }

     EnvMap map;
     map.name = strdup(node->name);
     map.val = NULL;
     env_vec[env_count] = map;
     env_count++;

     return 0;
}

int env_init() {
    env_node = mount_fs("env", "/", env_write, env_read, env_mkfile, env_mkdir, USER_PERMISSION);

    if (!env_node) {
        log::warning("env: could not create env filesystem");
        return 1;
    }

    create_file("PATH", "/env");
    create_file("HOME", "/env");

    setenv("PATH", "/bin:/usr/bin", 1);
    setenv("HOME", "/", 1);

    log::info("env: successfully created env filesystem!");
    return 0;
}
