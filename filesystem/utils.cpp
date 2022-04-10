#include "utils.h"

char *find_parent(char *_path) {
    char *path = strdup(_path);

    if (strcmp(path, "/") == 0) {
        return path;
    }

    if (path[strlen(path) - 1] == '/') {
        path[strlen(path) - 1] = 0;
    }

    for (int z = strlen(path) - 1; z >= 0; z--) {
        if (path[z] == '/')
            break;

        path[z] = 0;
    }

    return path;
}

char *find_name(char *path) {
    int last = 0;

    for (int z = 0; z < strlen(path); z++) {
        if (path[z] == '/') {
            last = z;
        }
    }

    char *name = strdup(path);
    memset(name, 0, strlen(path));
    int c = 0;

    for (int z = last + 1; z < strlen(path); z++) {
        name[c] = path[z];
        c++;
    }

    name[c] = 0;

    return name;
}
