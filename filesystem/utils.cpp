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
        if (path[z] == '/') {
            if (strcmp(path, "/")) {
                path[z] = 0;
            }
            break;
        }

        path[z] = 0;
    }

    log::warning("PATH: %s\n", path);

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

char *fixup_path(char *path, char *out) {
    char **buf;
    int c = 0;
    int last = 0;
    int size = tokenize(path, '/', buf);

    for (int z = 0; z < size; z++) {
        if (strcmp(buf[z], ".") == 0) {
            free(buf[z]);
            continue;
        } else if (strcmp(buf[z], "..") == 0) {
            c -= last;
            if (z != (size - 1)) c--;
            out[c] = NULL;

            free(buf[z]);
            continue;
        }

        last = strlen(buf[z]);

        for (int v = 0; v < last; v++) {
            out[c] = buf[z][v];
            c++;
        }

        out[c] = '/';
        c++;
    }

    c--;
    out[c] = NULL;

    return out;
}
