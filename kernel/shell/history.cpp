#include "history.h"
#include <filesystem/disk.h>

void hist_add(char *cmd) {
    history[hist_count] = cmd;
    hist_count++;
}

int hist_push() {
    if (hist_count == 0) {
        log::warning("history: history list empty.");
        return 1;
    }

    auto node = rbfs_open("/configs/sh/history");

    if (node) {
        log::error("history: failed to find '%s'!", "/configs/sh/history");
        return 1;
    }

    int length = 0;

    for (int z = 0; z < hist_count; z++) {
        length += strlen(history[z]);
        length += 2;
    }

    char contents[length + 1];
    memset(contents, 0, length + 1);

    strcpy(contents, history[0]);
    strcat(contents, "\0\n");

    for (int z = 1; z < hist_count; z++) {
        strcat(contents, history[z]);
        strcat(contents, "\0\n");
    }

    int ret = rbfs_write(contents, 0, length, node);

    if (ret) {
        log::error("history: failed to write to file.");
        return 1;
    }

    return 0;
}

void hist_readdisk() {
    int limit = 2000;

    auto node = rbfs_open("/configs/sh/history");

    if (!node) {
        log::error("history: failed to find file!");
        return;
    }

    char text[limit];
    memset(text, 0, limit);

    int ret = rbfs_read(text, 0, limit, node);

    if (ret) {
        log::error("history: failed to read file.");
        return;
    }

    hist_count = tokenize(text, '\n', history);
}

// last entered cmd first
char *hist_get(int pos) {
    if (pos < 0 || pos >= hist_count) {
        return NULL;
    }

    if (hist_count - 1 - pos < 0) {
        return NULL;
    }

    return history[hist_count - 1 - pos];
}

void hist_traverse() {
    printf("History: \n");

    for (int z = hist_count - 1; z >= 0; z--) {
        printf("\t%s\n", history[z]);
    }
}

void hist_init() {
    hist_readdisk();
}
