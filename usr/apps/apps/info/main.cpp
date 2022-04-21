#include <stdio.h>
#include <system.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    auto info = System::info();

    bool just_version = false, just_os = false, just_data = false;
    bool cont = false;
    bool rd = false;

    char redirect[100];

    text::copy(redirect, "/dev/stdout"); // default to stdout

    for (int z = 1; z < argc; z++) {
        if (cont && rd) {
            text::set(redirect, 0, 100);
            text::copy(redirect, argv[z]);
            cont = false;
            rd = false;
            continue;
        } else if (cont) {
            cont = false;
            continue;
        }

        if (text::compare(argv[z], "-v") == 0) {
            if (!just_os) just_version = true;
            continue;
        } else if (text::compare(argv[z], "-os") == 0) {
            if (!just_version) just_os = true;
            continue;
        } else if (text::compare(argv[z], "-d") == 0) {
            just_data = true;
            continue;
        } else if (text::compare(argv[z], ">") == 0) {
            if (z + 1 < argc) {
                cont = true;
                rd = true;
                continue;
            } else {
                text::write("Invalid redirection!\n");
                return 1;
            }
        }

        text::write("Unknown argument: %s\n", argv[z]);
        return 1;
    }

    auto file = Filesystem::open(redirect, "w");

    char *buffer = (char *)mem::alloc(100);
    text::set(buffer, 0, 100);

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

    Filesystem::write(buffer, 0, 100, file);

    Filesystem::close(file);

    return 0;
}
