#include <filesystem/parse.h>

int get_slash_in_path(char *path)
{
    int count = 0;

    for (int z = 0; z < strlen(path); z++)
    {
        if (path[z] == '/')
            count++;
    }

    return count;
}
