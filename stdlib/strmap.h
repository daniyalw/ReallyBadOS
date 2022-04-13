#include <string.h>
#include <ctype.h>

namespace std {

class strmap : map<char *> {
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

            push_back(name, val);
        }
    }

    strmap() {}
    strmap(char *str) {
        parse(str);
    }
};

}
