#include <fs.h>

using namespace Filesystem;
using namespace Ramdisk;
using namespace std;

path_t parse_name(char * path)
{
    int length = std::len(path);
    char folder[length];
    int fdz = 0;
    char fname[length];
    int fnz = 0;

    bool first = false;

    for (int z = 0; z < length; z++)
    {
        folder[z] = 0;
        fname[z] = 0;
    }

    for (int z = 0; z < length; z++)
    {
        if (path[z] == '/')
        {
            first = true;
        }
        else
        {
            if (first)
            {
                fname[fnz] = path[z];
                fnz++;
            }
            else
            {
                folder[fdz] = path[z];
                fdz++;
            }
        }
    }

    path_t _path;

    for (int z = 0; z < 128; z++)
    {
        _path.filename[z] = 0;
        _path.foldername[z] = 0;
    }

    for (int z = 0; z < std::len(fname); z++)
        _path.filename[z] = fname[z];

    for (int z = 0; z < std::len(folder); z++)
        _path.foldername[z] = folder[z];

    return _path;
}

namespace Filesystem
{

namespace Ramdisk
{

RFILE null_file()
{
    RFILE f;
    f.null = true;
    return f;
}

RFOLDER null_folder()
{
    RFOLDER f;
    f.null = true;
    return f;
}

RFOLDER get_folder(char * name)
{
    for (int z = 0; z < Filesystem::Ramdisk::folder_count; z++)
        if (strcmp(Filesystem::Ramdisk::folders[z].name, name))
            return Filesystem::Ramdisk::folders[z];

    return null_folder();
}

RFILE get_file(char * name, char * folder)
{
    RFOLDER parent = get_folder(folder);

    if (parent.null)
        return null_file();

    for (int z = 0; z < parent.file_count; z++)
        if (strcmp(parent.files[z].name, name))
            return parent.files[z];

    return null_file();
}

RFOLDER create_folder(char * name)
{
    if (!get_folder(name).null) {
        return null_folder();
    }
    RFOLDER folder;
    folder.name = name;
    folder.path = std::get("/%s", name);
    folder.id = Filesystem::Ramdisk::folder_count;

    Filesystem::Ramdisk::folders[folder.id] = folder;
    Filesystem::Ramdisk::folder_count++;

    return folder;
}

RFILE create_file(char * name, char * folder, char * contents)
{
    if (!get_file(name, folder).null) {
        return null_file();
    }
    RFILE file;

    file.name = name;
    file.parent = folder;
    file.contents = contents;
    file.path = std::get("/%s/%s", folder, name);

    RFOLDER parent = get_folder(folder);

    if (parent.null)
        return null_file();

    file.id = parent.file_count;
    parent.files[file.id] = file;
    parent.file_count++;
    Filesystem::Ramdisk::folders[parent.id] = parent;

    return file;
}

void __fs_test()
{
    create_folder("f1");
    create_file("abc.txt", "f1", "sucks to be you");
    create_file("bb.txt", "f1", "NO");

    create_folder("f2");
    create_file("abc.txt", "f2", "dfasfds");

    ls();
}

}

void ls()
{
    printf("Folder count: %d\n\n", Filesystem::Ramdisk::folder_count);

    for (int z = 0; z < Filesystem::Ramdisk::folder_count; z++)
    {
        printf("/");
        printf(Filesystem::Ramdisk::folders[z].name);

        for (int b = 0; b < Filesystem::Ramdisk::folders[z].file_count; b++)
        {
            printf("\n");

            printf("/");
            printf(Filesystem::Ramdisk::folders[z].files[b].parent);
            printf("/");
            printf(Filesystem::Ramdisk::folders[z].files[b].name);

            printf("\n");

            //printf("\t\t%s\n", Filesystem::Ramdisk::folders[z].files[b].contents);
        }

        printf("\n");
    }

#ifdef DEBUG
    for (int z = 0; z < Filesystem::Ramdisk::folder_count; z++)
    {
        Kernel::system_log("/%s\n", Filesystem::Ramdisk::folders[z].name);

        for (int b = 0; b < Filesystem::Ramdisk::folders[z].file_count; b++)
        {
            Kernel::system_log("/%s/%s\n", Filesystem::Ramdisk::folders[z].files[b].parent, Filesystem::Ramdisk::folders[z].files[b].name);

            //printf("\t\t%s\n", Filesystem::Ramdisk::folders[z].files[b].contents);
        }

        Kernel::system_log("\n");
    }
#endif
}

}
