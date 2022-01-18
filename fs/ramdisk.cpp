#include <fs.h>
#include <memory.h>

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

namespace Ramdisk
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
    for (int z = 0; z < Filesystem::Ramdisk::folder_count; z++)
        if (strcmp(Filesystem::Ramdisk::folders[z].name, name))
            return Filesystem::Ramdisk::folders[z];

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
    if (!get_folder(name).null) {
        return null_folder();
    }
    FOLDER folder;
    folder.name = name;
    folder.path = std::get("/%s", name);
    folder.id = Filesystem::Ramdisk::folder_count;

    Filesystem::Ramdisk::folders[folder.id] = folder;
    Filesystem::Ramdisk::folder_count++;

    return folder;
}

FILE create_file(char * name, char * folder, char * contents, uint32_t size)
{
    if (!get_file(name, folder).null) {
        return null_file();
    }
    FILE file;

    file.name = name;
    file.parent = folder;
    file.contents = contents;
    file.path = std::get("/%s/%s", folder, name);
    file.size = size;

    FOLDER parent = get_folder(folder);

    if (parent.null) {
        return null_file();
    }

    file.id = parent.file_count;
    parent.files[file.id] = file;
    parent.file_count++;
    Filesystem::Ramdisk::folders[parent.id] = parent;

    return file;
}

FILE create_file(char * name, char * folder, char *( *_read)(), void (*_write)(char*))
{
    if (!get_file(name, folder).null) {
        return null_file();
    }

    FILE file;

    file.name = name;
    file.parent = folder;
    file.path = std::get("/%s/%s", folder, name);

    file.read = _read;
    file.write = _write;

    FOLDER parent = get_folder(folder);

    if (parent.null)
        return null_file();

    file.id = parent.file_count;
    parent.files[file.id] = file;
    parent.file_count++;
    Filesystem::Ramdisk::folders[parent.id] = parent;

    return file;
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

FILE fopen(char * name)
{
    path_t path = parse_name(name);
    return get_file(path.filename, path.foldername);
}
