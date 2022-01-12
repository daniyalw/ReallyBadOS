#include <fs.h>

using namespace Filesystem;
using namespace Ramdisk;
using namespace VFS;
using namespace std;

namespace Filesystem
{

namespace VFS
{

FILE f_null_file()
{
    FILE f;
    f.null = true;
    return f;
}

FILE fcreate(TarFile block)
{
    FILE file;
    file.name = block.name;

    for (int z = 0; z < all_files.size(); z++)
    {
        if (strcmp(block.name, all_files[z].name))
            return f_null_file();
    }

    for (int z = 0; z < 8; z++)
    {
        file.gid[z] = block.gid[z];
        file.uid[z] = block.uid[z];
    }

    file.size = block.size;
    file.contents = block.contents;

    return file;
}

void save_file(FILE f)
{
    for (int z = 0; z < all_files.size(); z++)
    {
        if (all_files[z].name == f.name)
        {
            all_files[z] = f;
            return;
        }
    }
}

void list_dir()
{
    for (int z = 0; z < all_files.size(); z++)
    {
        printf("Name: %s\n", all_files[z].name);
    }
}

FILE get_file_n(char * name)
{
    for (int z = 0; z < all_files.size(); z++)
    {
        if (strcmp(all_files[z].name, name))
            return all_files[z];
    }

    FILE f;
    f.null = true;
    return f;
}

// get the position of a file in the list
int get_file_z(char * name)
{
    for (int z = 0; z < all_files.size(); z++)
    {
        if (strcmp(all_files[z].name, name))
            return z;
    }

    return -1;
}

char *read(char * fname)
{
    FILE f = get_file_n(fname);

    if (!f.null)
        return f.contents;
    else
        return NULL;
}

FILE fopen(char * name)
{
    return get_file_n(name);
}

void close(char *name)
{
    UNUSED(name);

    // not implemented
}

void write(char * name, char * data)
{
    FILE file = fopen(name);

    file.contents = data;

    int z = get_file_z(name);

    if (z != -1)
    {
        all_files.replace(file, z);
    }
}

}

}
