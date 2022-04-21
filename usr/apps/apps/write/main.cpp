#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getlength(int start, int argc, char *args[])
{
    int length = 0;

    for (int z = start; z < argc; z++)
        length += text::len(args[z]);

    return length;
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        if (text::compare(argv[1], "-h") == 0)
        {
            text::write("Help: write <file-path> <contents>\n");
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

    if (argc < 3)
    {
        text::write("Help: write <file-path> <contents>");
        return 1;
    }

    const int length = getlength(2, argc, argv);

    char contents[length];
    int cz = 4;
    text::set(contents, 0, length);

    for (int z = 2; z < argc; z++)
    {
        for (int b = 0; b < text::len(argv[z]); b++)
        {
            contents[cz] = argv[z][b];
            cz++;
        }

        contents[cz] = ' ';
        cz++;
    }

    contents[cz] = 0;

    char *c = (char *)&contents[4];

    text::write("To write: '%s'", c);

    FILE *file = Filesystem::open(argv[1], "w");
    Filesystem::write(c, 1, 10, file);
    Filesystem::close(file);

    return 0;
}
