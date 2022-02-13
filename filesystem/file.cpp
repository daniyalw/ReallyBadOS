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

int create_file(char *path, char *folder, char *contents)
{
    int id = find_id(folder);

    if (id < 0)
        return 1;

    fs_node node = create_node(path, id, FS_NODE_FILE);

    if (node.null)
        return 1;

    node_write_basic(node.id, contents);

    return 0;
}

int create_file(char *path, char *folder, __read read, __write write)
{
    int parent_id = find_id(folder);

    if (parent_id < 0)
        return 1;

    fs_node node = create_node(path, parent_id, FS_NODE_FILE);

    if (node.null)
        return 1;

    node.write = write;
    node.read = read;

    nodes[node.id] = node;

    return 0;
}

void fprintf(FILE *file, char *data)
{
    if (file->write != NULL)
    {
        file->write(data);
    }
    else
    {
        node_write_basic(file->node.id, data);
        file->node = nodes[file->node.id];
    }
}

void fprintf(FILE file, char *data)
{
    if (file.write != NULL)
    {
        file.write(data);
    }
    else
    {
        node_write_basic(file.node.id, data);
        file.node = nodes[file.node.id];
    }
}

void fprintf(fs_node node, char *data)
{
    if (node.write != NULL)
    {
        node.write(data);
    }
    else
    {
        node_write_basic(node.id, data);
        node = nodes[node.id];
    }
}
