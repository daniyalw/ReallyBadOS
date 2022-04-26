#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            text::write("Help: read <file-name>\n");
            text::write("Options: \n"
                    " -h help\n"
                    " -v version info");
            return 0;
        }
        else if (text::compare(argv[1], "-v") == 0)
        {
            text::write("Version: 0.2");
            return 0;
        }
    }

    if (argc < 2)
    {
        text::write("Help: read <file-name>\n");
        return 1;
    }

    auto *file = Filesystem::open(argv[1], "r");

    if (!file)
    {
        printf("File is null.\n");
    }
    else
    {
        char *buf = (char *)mem::alloc(100);
        text::set(buf, 0, 100);

        printf("Name: %s\n", file->node->name);
        printf("Size: %d bytes\n", file->node->get_size(file->node));

        int ret = Filesystem::read(buf, file->node->get_size(file->node), 1, file);

        if (ret) {
            text::write("Could not read from file %s\n", file->node->path);
            Filesystem::close(file);
            return 0;
        }

        bool issp = true;

        if (!buf)
            text::copy(buf, "(null)");

        for (int z = 0; z < strlen(buf); z++)
            if (buf[z] != ' ')
                issp = false;
        text::write("Read: %s\n", (char *)(issp ? "(space)" : buf));

        mem::release(buf);
    }

    Filesystem::close(file);

    return 0;
}
