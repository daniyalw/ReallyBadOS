#include <filesystem/node.h>
#include <string.h>

fs_node null_node()
{
    fs_node node;
    node.null = true;
    return node;
}

fs_node find_node(int id)
{
    for (int z = 0; z < node_count; z++)
    {
        if (nodes[z].id == id)
            return nodes[z];
    }

    return null_node();
}

fs_node create_node(char *name, int parent, int flags)
{
    fs_node throwaway = find_node(parent);
    char *par;
    fs_node test = find_node(find_id(get(par, "%s%s", throwaway.path, name)));
    if (!test.null)
    {
        char *a;
        test = find_node(find_id(get(a, "%s%s/", throwaway.path, name)));

        if (!test.null)
            return null_node();
    }
    fs_node node;

    strcpy(node.name, name);

    node.parent_id = parent;
    node.id = node_count;

    node.flags = flags;

    if (parent < 0)
    {
        strcpy(node.path, name);
    }
    else
    {
        fs_node par = find_node(parent);

        // if the parent specified is not a folder
        if (par.flags != FS_NODE_FOLDER)
            return null_node();

        if (flags == FS_NODE_FOLDER)
            strcpy(node.path, get("", "%s%s/", par.path, name));
        else
            strcpy(node.path, get("", "%s%s", par.path, name));

        par.children_id[par.children_count] = node.id;
        par.children_count++;

        nodes[par.id] = par;
    }

    nodes[node_count] = node;
    node_count++;

    return node;
}

void fs_ls(int id, int index)
{
    fs_node node = find_node(id);

    if (node.null)
        return;

    for (int z = 0; z < index; z++)
        log::info("    ");

    log::info("%s%s", node.name, (char *)(node.flags == FS_NODE_FILE ? " (file)" : " (folder)"));

    for (int z = 0; z < index; z++)
        printf("    ");

    printf("%s%s\n", node.name, (char *)(node.flags == FS_NODE_FILE ? " (file)" : " (folder)"));

    index++;

    for (int z = 0; z < node.children_count; z++)
        fs_ls(node.children_id[z], index);
}

void fs_ls(char *path)
{
    fs_node node = find_node(find_id(path));

    if (node.null)
    {
        return;
    }
    else
        fs_ls(node.id, 0);
}

void ls_root()
{
    fs_ls(find_id("/"), 0);
}

// the next two functions are basic handlers for usual files
// in case the file isn't a device or something like that
void node_write_basic(int id, char *contents)
{
    fs_node node = find_node(id);

    if (node.null || node.flags != FS_NODE_FILE)
        return;

    node.contents = contents;
    node.size = strlen(contents) * sizeof(char);

    nodes[node.id] = node;
}

void node_write_append(int id, char *contents)
{
    fs_node node = find_node(id);

    if (node.null || node.flags != FS_NODE_FILE)
        return;

    node.contents = append(node.contents, contents, node.contents);
    node.size += strlen(contents);

    nodes[node.id] = node;
}

char *node_read_basic(int id)
{
    fs_node node = find_node(id);

    if (node.null || node.flags != FS_NODE_FILE)
        return NULL;

    return node.contents;
}

char *find_name(int id)
{
    for (int z = 0; z < node_count; z++)
        if (nodes[z].id == id)
            return nodes[z].name;

    return NULL;
}

int find_id(char *path)
{
    for (int z = 0; z < node_count; z++)
        if (strcmp(nodes[z].path, path) == 0)
            return z;

    return -1;
}
