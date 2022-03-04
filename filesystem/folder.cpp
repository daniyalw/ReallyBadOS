#include <filesystem/folder.h>
#include <filesystem/parse.h>

bool no_more_after(char *name, int index)
{
    for (int z = index; z < strlen(name); z++)
        if (name[z] == '/')
            return true;

    return false;
}

int mkdir(char *name, char *parent)
{
    parent = format_folder(parent);
    int id = find_id(parent);

    if (id < 0)
    {
        // parent not found
        return 1;
    }

    fs_node node = create_node(name, id, FS_NODE_FOLDER);

    if (node.null)
        return 1;

    return 0;
}

int mkdir(char *path)
{
    if (startswith(path, "/"))
        path = &path[1];

    if (endswith(path, "/"))
        path[strlen(path)-1] = 0;

    char **buf;
    int ret;
    char *parent = NULL;
    int z = 0;
    char *folder = NULL;

    ret = tokenize(path, '/', buf);

    folder = buf[ret-1];

    while (z < ret - 1)
    {
        if (z == 0)
            parent = get(parent, "/%s/", buf[z]);
        else
            parent = get(parent, "%s%s/", parent, buf[z]);

        z++;
    }

    return mkdir(folder, parent);
}
