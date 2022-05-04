#include <clipboard.h>
#include <stdlib.h>

char *read_clipboard() {
    FILE *file = fopen("/configs/clipboard", "r");

    char *contents = (char *)malloc(100);
    memset(contents, 0, 100);
    fread(contents, 1, 100, file);

    fclose(file);
    return contents;
}

void write_clipboard(char *contents) {
    FILE *file = fopen("/configs/clipboard", "r");
    fwrite(contents, 1, 100, file);
    fclose(file);
}

#ifdef __cplusplus
namespace Clipboard {
    char *read() {
        return read_clipboard();
    }

    void write(char *contents) {
        return write_clipboard(contents);
    }
}
#endif
