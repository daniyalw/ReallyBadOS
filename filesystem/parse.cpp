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

char *parse_name_parent(char *path, char *out)
{
    const int length = strlen(path);
    int finding = 0;

    for (int z = length; z > 0; z--)
    {
        if (path[z] == '/')
        {
            break;
        }

        finding++;
    }

    char name[length];
    int nz = 0;

    for (int b = 0; b < finding; b++)
    {
        if (path[b] == '/')
            break;

        name[nz] = path[b];
        nz++;
    }

    out = (char *)name;

    return out;
}

char *parse_current_name(char *path, char *out)
{
    const int slash_count = get_slash_in_path(path);
    const int length = strlen(path);
    int slash_counted = 0;

    char name[length];
    int name_z = 0;

    for (int z = 0; z < length; z++)
    {
        if (path[z] == '/')
        {
            slash_counted++;
        }
        else
        {
            if (slash_counted >= slash_count)
            {
                name[name_z] = path[z];
                name_z++;
            }
        }
    }

    name[name_z] = 0;

    out = (char *)name;

    return out;
}

char *format_folder(char *path)
{
    if (path[0] != '/')
    {
        path = get("", "/%s", path);
    }
    
    return path;
}

char *format_file(char *path)
{
    if (path[0] != '/')
    {
        path = get("", "/%s", path);
    }

    return path;
}
