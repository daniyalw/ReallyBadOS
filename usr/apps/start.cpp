#include <env.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv);

extern "C" int start(int argc, char **argv, EnvMap *maps, int envc) {
    for (int z = 0; z < envc; z++) {
        putenv(maps[z].name);
        setenv(maps[z].name, maps[z].val, 0);
    }

    return main(argc, argv);
}
