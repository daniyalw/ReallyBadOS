#pragma once

#include <sys/time/time.h>

int seed;

int get_random_number() {
    int num = seed * 1103515245 + 12345 - (seconds * minute - hour);
    seed++;
    return num/345643;
}

void set_seed(int _seed) {
    seed = _seed;
}
