#include <filesystem/node.h>
#include <filesystem/file.h>

FILE *fopen(char *path)
{
    int id = find_id(path);

    if (id == -1)
        return NULL;

    fs_node node = find_node(id);

    if (node.flags != FS_NODE_FILE)
        return NULL;

    FILE *file = (FILE *)malloc(sizeof(FILE *));

    file->node = node;
    file->write = node.write;
    file->read = node.read;
    file->null = node.null;

    strcpy(file->name, node.name);

    return file;
}

void fclose(FILE *file)
{
    free(file);
}

void create_file(char *path, char *folder, char *contents)
{
    int id = find_id(folder);

    if (id < 0)
        return;

    fs_node node = create_node(path, id, FS_NODE_FILE);

    if (node.null)
        return;

    node_write_basic(node.id, contents);
}
