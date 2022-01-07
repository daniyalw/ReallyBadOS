#include <fs.h>

using namespace Filesystem::VFS;
using namespace std;

path_t parse_name(char * path)
{
    int length = std::len(path);
    char folder[length];
    int fdz = 0;
    char fname[length];
    int fnz = 0;

    bool first = false;
    bool second = false;

    for (int z = 0; z < length; z++)
    {
        folder[z] = 0;
        fname[z] = 0;
    }

    for (int z = 0; z < length; z++)
    {
        if (path[z] == '/')
        {
            if (first)
                second = true;
            else
                first = true;
        }
        else
        {
            if (second)
            {
                fname[fnz] = path[z];
                fnz++;
            }
            else if (first)
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

namespace VFS
{

FILE null_file()
{
    FILE f;
    f.null = true;
    return f;
}

FOLDER null_folder()
{
    FOLDER f;
    f.null = true;
    return f;
}

FOLDER get_folder(char * name)
{
    for (int z = 0; z < Filesystem::VFS::folder_count; z++)
        if (strcmp(Filesystem::VFS::folders[z].name, name))
            return Filesystem::VFS::folders[z];

    return null_folder();
}

FILE get_file(char * name, char * folder)
{
    FOLDER parent = get_folder(folder);

    if (parent.null)
        return null_file();

    for (int z = 0; z < parent.file_count; z++)
        if (strcmp(parent.files[z].name, name))
            return parent.files[z];

    return null_file();
}

FOLDER create_folder(char * name)
{
    FOLDER folder;
    folder.name = name;
    folder.path = std::get("/%s", name);
    folder.id = Filesystem::VFS::folder_count;

    Filesystem::VFS::folders[folder.id] = folder;
    Filesystem::VFS::folder_count++;

    return folder;
}

FILE create_file(char * name, char * folder, char * contents)
{
    FILE file;

    file.name = name;
    file.parent = folder;
    file.contents = contents;
    file.path = std::get("/%s/%s", folder, name);

    FOLDER parent = get_folder(folder);

    if (parent.null)
        return null_file();

    file.id = parent.file_count;
    parent.files[file.id] = file;
    parent.file_count++;
    Filesystem::VFS::folders[parent.id] = parent;

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
    printf("Folder count: %d\n\n", Filesystem::VFS::folder_count);

    for (int z = 0; z < Filesystem::VFS::folder_count; z++)
    {
        printf("/%s\n", Filesystem::VFS::folders[z].name);

        for (int b = 0; b < Filesystem::VFS::folders[z].file_count; b++)
        {
            printf("/%s/%s\n", Filesystem::VFS::folders[z].files[b].parent, Filesystem::VFS::folders[z].files[b].name);

            //printf("\t\t%s\n", Filesystem::VFS::folders[z].files[b].contents);
        }

        printf("\n");
    }

#ifdef DEBUG
    for (int z = 0; z < Filesystem::VFS::folder_count; z++)
    {
        Kernel::system_log("/%s\n", Filesystem::VFS::folders[z].name);

        for (int b = 0; b < Filesystem::VFS::folders[z].file_count; b++)
        {
            Kernel::system_log("/%s/%s\n", Filesystem::VFS::folders[z].files[b].parent, Filesystem::VFS::folders[z].files[b].name);

            //printf("\t\t%s\n", Filesystem::VFS::folders[z].files[b].contents);
        }

        Kernel::system_log("\n");
    }
#endif
}

}
