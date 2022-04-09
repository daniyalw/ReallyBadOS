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
