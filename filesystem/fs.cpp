// my new vfs

#include <string.h>
#include <filesystem/node.h>
#include <filesystem/file.h>
#include <filesystem/parse.h>

void init_fs()
{
    root = create_node("/", -1, FS_NODE_FOLDER);

    fs_node dev = create_node("dev", root.id, FS_NODE_FOLDER);
    fs_node usr = create_node("usr", root.id, FS_NODE_FOLDER);
    fs_node apps = create_node("apps", root.id, FS_NODE_FOLDER);
    fs_node home = create_node("home", root.id, FS_NODE_FOLDER);
    fs_node trash = create_node("trash", root.id, FS_NODE_FOLDER);

    fs_node docs = create_node("documents", usr.id, FS_NODE_FOLDER);
}
