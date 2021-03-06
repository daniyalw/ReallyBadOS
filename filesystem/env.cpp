#pragma once

#include "env.h"

char *getenv(char *name) {
    char *path = (char *)malloc(PATH_LIMIT);
    memset(path, 0, PATH_LIMIT);
    get(path, "/env/%s", name);

    FILE *file = fopen(path, "r");

    if (!file) {
        return NULL;
    }

    int size = fsize(file);

    char *buffer = (char *)malloc(size + 1);
    memset(buffer, 0, size + 1);
    fread(buffer, 1, size, file);

    free(path);
    fclose(file);

    return buffer;
}

int setenv(char *name, char *val, int overwrite) {
    if (overwrite) {
        char *path = (char *)malloc(PATH_LIMIT);
        memset(path, 0, PATH_LIMIT);
        get(path, "/env/%s", name);

        FILE *file = fopen(path, "w");

        if (!file) {
            return NULL;
        }

        int ret = fwrite(val, 0, strlen(val), file);

        free(path);
        fclose(file);

        return ret;
    }

    return 0;
}

int putenv(char *envvar) {
    const int length = strlen(envvar);
    int loc = -1;
    // search for =
    for (int z = 0; z < length; z++) {
        if (envvar[z] == '=') {
            loc = z;
            break;
        }
    }

    if (loc == -1) {
        // could not find
        errno = EINVAL;
        return -1;
    }

    char name[loc + 1];
    memset(name, 0, loc + 1);
    strncpy(name, envvar, loc);

    char *ret = getenv(name);

    if (!ret) {
        create_file(name, "/env");
    }

    char contents[length];
    memset(contents, 0, length);
    strcpy(contents, &envvar[loc + 1]);

    char *c = strdup(contents);
    setenv(name, c, 1);
    free(c);

    return 0;
}

int append_env(char *name, char *val) {
    char *env = getenv(name);

    if (env) {
        strcat(env, val);
        return setenv(name, env, 1);
    }

    return 1;
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

fs_driver_t env_driver = {
    NULL,
    NULL,
    env_write,
    env_read,
    env_mkfile,
    env_mkdir
};

int env_init() {
    env_node = mount_fs("env", "/", env_driver, USER_PERMISSION);

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
