#include <stdio.h>
#include <system.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    info_t info = get_sys_info();

    bool just_version = false, just_os = false, just_data = false;
    bool cont = false;
    bool rd = false;

    char redirect[100];

    strcpy(redirect, "/dev/stdout"); // default to stdout

    for (int z = 1; z < argc; z++) {
        if (cont && rd) {
            memset(redirect, 0, 100);
            strcpy(redirect, argv[z]);
            cont = false;
            rd = false;
            continue;
        } else if (cont) {
            cont = false;
            continue;
        }

        if (strcmp(argv[z], "-v") == 0) {
            if (!just_os) just_version = true;
            continue;
        } else if (strcmp(argv[z], "-os") == 0) {
            if (!just_version) just_os = true;
            continue;
        } else if (strcmp(argv[z], "-d") == 0) {
            just_data = true;
            continue;
        } else if (strcmp(argv[z], ">") == 0) {
            if (z + 1 < argc) {
                cont = true;
                rd = true;
                continue;
            } else {
                printf("Invalid redirection!\n");
                return 1;
            }
        }

        printf("Unknown argument: %s\n", argv[z]);
        return 1;
    }

    FILE *file = fopen(redirect, "w");

    char *buffer = (char *)malloc(100);
    memset(buffer, 0, 100);

    if (!just_data) {
        if (!just_version && !just_os) {
            sprintf(buffer, "OS: %s\nVersion: %s%s\n", info.name, info.version, (char *)(info.dev ? " Dev" : ""));
        } else if (!just_version) {
            sprintf(buffer, "OS: %s\n", info.name);
        } else if (!just_os) {
            sprintf(buffer, "Version: %s%s\n", info.version, (char *)(info.dev ? " Dev" : ""));
        }
    } else {
        if (!just_version && !just_os) {
            sprintf(buffer, "%s\n%s%s\n", info.name, info.version, (char *)(info.dev ? " Dev" : ""));
        } else if (!just_version) {
            sprintf(buffer, "%s\n", info.name);
        } else if (!just_os) {
            sprintf(buffer, "%s%s\n", info.version, (char *)(info.dev ? " Dev" : ""));
        }
    }

    fwrite(buffer, 0, 100, file);

    fclose(file);

    return 0;
}
