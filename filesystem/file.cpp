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
    file->eof = NULL;
    file->ptr = NULL;

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
    fprintf(file->node, data);
}

void fprintf(FILE file, char *data)
{
    fprintf(file.node, data);
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

int fwrite(char *buf, int offset, int size, FILE *file)
{
    if (file == NULL) return 1;

    if (file->node.write)
        file->node.write(file->node, offset, size, buf);
    else
        node_write_basic(file->node.id, buf);

    file->node = find_node(file->node.id);

    return 0;
}

char * fread(char *buf, int offset, int size, FILE *file)
{
    if (file->node.read)
        buf = file->node.read(file->node, offset, size, buf);
    else
    {
        char *tmp = node_read_basic(file->node.id);
        strcpy(buf, &tmp[offset]);
        buf[size] = 0;
    }
    return buf;
}

char *fread(char *buf, FILE *file)
{
    return fread(buf, 0, file->node.size, file);
}

int fgetc(FILE *file)
{
    char *str = fread(str, file->ptr, 1, file);
    file->ptr++;

    if (file->ptr == file->node.size)
        file->eof = EOF;

    return str[0];
}

int feof(FILE *file)
{
    return file->eof < 0;
}

char *fgets(char *str, int n, FILE *file)
{
    char c;
    int z = 0;

    while (true)
    {
        if (z == n || feof(file))
            break;

        c = fgetc(file);

        str[z] = c;
        z++;
    }

    str[z] = NULL;

    if (z == 0)
        return NULL;

    return str;
}

int fgetpos(FILE *file, fpos_t *pos)
{
    *pos = file->ptr;

    return *pos;
}

int fseek(FILE *file, int offset, int w)
{
    if (file == NULL)
        return 1;

    file->ptr = offset + w;
    return 0;
}

int ftell(FILE *file)
{
    int offset[1];
    offset[0] = fgetpos(file, offset);
    return offset[0];
}

void rewind(FILE *file)
{
    fseek(file, 0, 0);
}

int fsetpos(FILE *file, fpos_t *pos)
{
    file->ptr = *pos;
    return file->ptr;
}
