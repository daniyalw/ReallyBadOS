#pragma once

#include "_header.h"

C_HEADER

extern char *read_clipboard();
extern void write_clipboard(char *contents);

C_HEADER_END

#ifdef __cplusplus
namespace Clipboard {
    char *read();
    void write(char *contents);
}
#endif
