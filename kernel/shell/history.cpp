#include "history.h"
#include <filesystem/disk.h>

void hist_add(char *cmd) {
    history[hist_count] = cmd;
    hist_count++;
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
}
