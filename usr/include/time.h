#pragma once

#include "_header.h"

C_HEADER

typedef struct {
    int sec, min, h, d, m, y, wd;
    bool pm;
} time_t;

extern time_t get_time();

C_HEADER_END
