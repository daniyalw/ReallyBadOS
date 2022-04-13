#include <string.h>
#include <ctype.h>

namespace std {

class map {
    typedef struct {
        uint32_t start;
        char *name;
        char *val;
    } MapKey;

    MapKey *keys;
    int mkey_count = 0;

public:
    void parse(char *str) {
        char **buf;
        int size = tokenize(str, ';', buf);

        for (int z = 0; z < size; z++) {
            char *name = buf[z];
            char *val;

            int c = 0;
            while (isalpha(name[c])) {
                c++;
            }

            int n = -1;
            for (int v = 0; v < strlen(buf[z]); v++) {
                if (buf[z][v] == ':') {
                    n = v + 1;
                    break;
                }
            }

            if (n == -1) {
                for (int m = 0; m < size; m++) {
                    free(buf[m]);
                }

                return;
            } else {
                int max = strlen(buf[z]);

                val = &buf[z][n];
                val[max] = 0;
            }

            name[c] = 0;

            printf("NAME: %s\nVAL1: %s\n", name, val);

            MapKey key;
            key.start = (uint32_t)&buf[z];
            key.name = name;
            key.val = val;

            keys[mkey_count] = key;
            mkey_count++;
        }
    }

    map() {}
    map(char *str) {
        parse(str);
    }

    char *find(char *name) {
        for (int z = 0; z < mkey_count; z++) {
            if (strcmp(keys[z].name, name) == 0) {
                return keys[z].val;
            }
        }

        return NULL;
    }
};

}
