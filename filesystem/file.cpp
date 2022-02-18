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
    folder = format_folder(folder);
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
    folder = format_folder(folder);
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
    file->write(data);
}

void fprintf(FILE file, char *data)
{
    file.write(data);
}

void fprintf(fs_node node, char *data)
{
    if (node.write != NULL)
    {
        node.write(node, 0, node.size, data);
    }
    else
    {
        node_write_basic(node.id, data);
        node = nodes[node.id];
    }
}

int file_write(FILE *file, char *buf)
{
    if (file == NULL) return 1;

    if (file->node.write)
        file->node.write(file->node, 0, file->node.size, buf);
    else
        node_write_basic(file->node.id, buf);

    file->node = find_node(file->node.id);

    return 0;
}

char * file_read(FILE *file, char *buf)
{
    if (file->node.read)
        buf = file->node.read(file->node, 0, file->node.size, buf);
    else
        buf = node_read_basic(file->node.id);
    return buf;
}
