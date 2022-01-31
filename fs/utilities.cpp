#include <fs/fs.h>

using namespace Filesystem;
using namespace Ramdisk;

void copy_file(char *orig_folder, char *orig_name, char *to_folder)
{
    FILE file = get_file(orig_name, orig_folder);

    create_file(orig_name, to_folder, file.contents, file.size);
}

void delete_file(char *folder_c, char *file_c)
{
    FILE file = get_file(file_c, folder_c);
    FOLDER folder = get_folder(folder_c);

    if (folder.null || file.null)
        return;

    file.null = true;
    folder.files[file.id] = file;
    folders[folder.id] = folder;
}

void delete_folder(char *foldername)
{
    FOLDER folder = get_folder(foldername);

    if (folder.null)
        return;

    folder.null = true;

    folders[folder.id] = folder;
}

void rename_file(char *folder_name, char *name, char *newname)
{
    FILE file = get_file(name, folder_name);
    FOLDER folder = get_folder(folder_name);

    if (folder.null || file.null)
        return;

    file.name = newname;
    folder.files[file.id] = file;
    folders[folder.id] = folder;
}

void rename_folder(char *foldername, char *newname)
{
    FOLDER folder = get_folder(foldername);

    if (folder.null)
        return;

    folder.name = newname;
    folder.path = std::get("/%s", newname);

    for (int z = 0; z < folder.file_count; z++)
    {
        FILE file = folder.files[z];

        if (file.null)
            continue;

        file.parent = newname;
        file.path = std::get("/%s/%s", newname, file.name);

        folder.files[z] = file;
    }

    folders[folder.id] = folder;
}

void move_file(char *old_folder, char *old_name, char *to_folder, char *to_name)
{
    copy_file(old_folder, old_name, to_folder);
    delete_file(old_folder, old_name);
    rename_file(to_folder, old_name, to_name);
}
