#include "disk.h"
#include <drivers/disk/disk.h>
#include "utils.h"

void rbfs_format() {
    RBFSSuperblock super;

    super.magic = RBFS_DISK_MAGIC;
    super.disk_size = rbfs_disk->total_sectors() * 512;
    super.status = RBFS_CLEAN;
    super.files = 0;
    super.first_free = RBFS_BEG + 1;

    rbfs_disk->write(RBFS_BEG, (uint8_t *)&super);
}

void add_index(char *name, char *path, int sector, int type) {
    RBFSIndex *index = new RBFSIndex();

    memset(index->name, 0, 20);
    memset(index->path, 0, 100);
    strcpy(index->name, name);
    strcpy(index->path, path);

    index->sector = sector;
    index->type = type;

    indexed[index_count] = index;
    index_count++;
}

void wipe_disk() {
    find_disk();
    rbfs_format(); // just ignore the files
}

int rbfs_create(char *name, char *parent, int type, int permission, char *contents) {
    char *path;
    if (strcmp(name, "/") == 0 && strcmp(parent, "/") == 0) {
        path = strdup("/");
    } else {
        path = (char *)malloc(strlen(name) + strlen(parent) + 1);
        memset(path, 0, strlen(name) + strlen(parent) + 1);

        if (parent[strlen(parent) - 1] == '/') {
            if (type == RBFS_DIR) {
                if (strcmp(parent, "/") == 0) {
                    sprintf(path, "%s%s", parent, name);
                } else {
                    sprintf(path, "%s/%s", parent, name);
                }
            } else {
                sprintf(path, "%s/%s", parent, name);
            }
        }
    }

    RBFSNode *check = find_file(path);

    if (check != NULL) {
        // make sure node doesn't exist
        free(check);
        free(path);
        return 1;
    }

    if (strcmp(parent, "/") != 0) {
        RBFSNode *par = find_file(parent);

        if (par == NULL) {
            free(path);
            return 1;
        } else {
            free(par);
        }
    }

    RBFSNode node;

    memset(node.name, 0, 20);
    memset(node.path, 0, 100);
    strncpy(node.name, name, 20);
    strncpy(node.path, path, 100);

    node.uid = 0;
    node.gid = 0;
    node.type = type;
    node.permission = permission;
    node.error = RBFS_CLEAN;
    node.magic = RBFS_DISK_MAGIC;

    int sectors = 0;

    if (!contents) {
        node.sectors = 0;
    } else {
        sectors = strlen(contents)/512 + 1;
        node.sectors = sectors;
    }

    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);

    rbfs_disk->read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    if (super->magic != RBFS_DISK_MAGIC) {
        free(bytes);
        return 1;
    }

    int sector = super->first_free;

    rbfs_disk->write(super->first_free, (uint8_t *)&node);
    DiskDrivers::ATA::ata_write(super->first_free + 1, sectors, (uint8_t *)contents);

    super->first_free += 1 + sectors;
    rbfs_disk->write(RBFS_BEG, (uint8_t *)super);

    add_index(node.name, node.path, sector, node.type);

    free(super);

    return 0;
}

int rbfs_create_file(char *path, char *contents) {
    char *name = find_name(path);
    char *parent = find_parent(path);

    int ret = rbfs_create(name, parent, RBFS_FILE, 0, contents);

    return ret;
}

int rbfs_create_dir(char *path) {
    if (strcmp(path, "/") == 0) {
        rbfs_create("/", "/", RBFS_DIR, 0, NULL);
        return 1;
    }

    char *name = find_name(path);
    char *parent = find_parent(path);

    int ret = rbfs_create(name, parent, RBFS_DIR, 0, NULL);

    return ret;
}

void index_disk() {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);

    rbfs_disk->read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    if (super->magic != RBFS_DISK_MAGIC) {
        free(bytes);
        return;
    }

    int last_sector = super->first_free;
    int z = RBFS_BEG + 1;

    while (z < last_sector) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);

        rbfs_disk->read(b, z, 1);
        RBFSNode *node = (RBFSNode *)b;

        if (node->magic != RBFS_DISK_MAGIC) {
            free(b);
            continue;
        }

        add_index(node->name, node->path, z, node->type);

        z++;
        z += node->sectors;

        if (strcmp(node->path, "/") == 0) {
            free(b);
            continue;
        }

        char *parent = find_parent(node->path);

        char *par = (char *)malloc(strlen(parent) + 10);
        memset(par, 0, strlen(parent) + 10);
        sprintf(par, "/disk0%s", parent);

        if (!endswith(node->path, "/")) {
            create_file(node->name, par);
        } else {
            make_dir(node->name, par);
        }

        free(b);
    }

    free(bytes);
}

RBFSNode *find_file(char *path) {
    for (int z = 0; z < index_count; z++) {
        if (strcmp(indexed[z]->path, path) == 0) {
            int sector = indexed[z]->sector;

            uint8_t *bytes = (uint8_t *)malloc(512);
            memset(bytes, 0, 512);

            rbfs_disk->read(bytes, sector, 1);

            return (RBFSNode *)bytes;
        }
    }

    return NULL;
}

RBFSIndex *find_index(char *path) {
    for (int z = 0; z < index_count; z++) {
        if (strcmp(indexed[z]->path, path) == 0) {
            return indexed[z];
        }
    }

    return NULL;
}

RBFSIndex *find_index(RBFSNode *node) {
    return find_index(node->path);
}

int find_sector_node(RBFSNode *node) {
    if (find_index(node)) {
        return find_index(node)->sector;
    }

    return -1;
}

char *read_file(RBFSNode *node, int offset, int sectors) {
    if (node == NULL) {
        return NULL;
    }

    if (node->magic != RBFS_DISK_MAGIC) {
        return NULL;
    }

    uint8_t *bytes = (uint8_t *)malloc(sectors * 512);
    memset(bytes, 0, sectors * 512);

    rbfs_disk->read(bytes, find_sector_node(node) + offset + 1, sectors);

    char *contents = (char *)malloc(sectors * 512 + 1);
    memset(contents, 0, sectors * 512 + 1);

    for (int z = 0; z < sectors * 512; z++) {
        contents[z] = (char)bytes[z];
    }

    free(bytes);

    return contents;
}

char *read_file(RBFSNode *node) {
    return read_file(node, 0, node->sectors);
}

char *read_file(char *path) {
    RBFSNode *node = find_file(path);

    if (node == NULL) {
        return NULL;
    }

    return read_file(node);
}

void move_sector_up(int sector) {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    rbfs_disk->read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = super->first_free - 1; z >= sector; z--) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        rbfs_disk->read(b, z, 1);
        rbfs_disk->write(z + 1, b);

        free(b);
    }

    super->first_free++;
    rbfs_disk->write(RBFS_BEG, (uint8_t *)super);

    free(super);
}

// sector can be overwritten
void move_sector_down(int sector) {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    rbfs_disk->read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = sector; z <= super->first_free; z++) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        rbfs_disk->read(b, z + 1, 1);
        rbfs_disk->write(z, b);

        free(b);
    }

    super->first_free--;
    rbfs_disk->write(RBFS_BEG, (uint8_t *)super);

    free(super);
}

void move_sectors_up(int sector, int sectors) {
    for (int z = 0; z < sectors; z++)
        move_sector_up(sector + z);
}

void move_sectors_down(int sector, int sectors) {
    for (int z = sectors; z >= 0; z--)
        move_sector_down(sector + z);
}

int rbfs_edit_file(RBFSNode *node, char *new_contents) {
    RBFSIndex *index = find_index(node->path);

    if (!index) {
        return 1;
    }

    int sectors = strlen(new_contents)/512 + 1;
    int contents_beg = index->sector + 1;

    if (sectors < node->sectors)
        move_sectors_down(contents_beg + (node->sectors - sectors), node->sectors - sectors);
    else if (sectors > node->sectors)
        move_sectors_up(contents_beg + node->sectors, sectors - node->sectors);

    node->sectors = sectors;

    DiskDrivers::ATA::ata_write(index->sector + 1, sectors, (uint8_t *)new_contents);
    rbfs_disk->write(index->sector, (uint8_t *)node);

    return 0;
}

int rbfs_vfs_write(fs_node_t *_node, int offset, int size, char *buf) {
    char *_path = strdup(_node->path);
    char *path;

    if (startswith(path, "/disk0")) {
        path = &_path[6];
    } else {
        path = &_path[0];
    }

    RBFSNode *node = find_file(path);

    free(_path);

    if (!node) {
        return 1;
    }

    return rbfs_edit_file(node, buf);
}

int rbfs_vfs_read(fs_node_t *_node, int offset, int size, char *buf) {
    char *_path = strdup(_node->path);
    char *path;

    if (startswith(path, "/disk0")) {
        path = &_path[6];
    } else {
        path = &_path[0];
    }

    RBFSNode *node = find_file(path);

    free(_path);

    if (!node) {
        return 1;
    }

    char *buffer = read_file(node);

    strcpy(buf, buffer);

    free(buffer);

    return 0;
}

int rbfs_vfs_mkfile(fs_node_t *node) {
    char *_path = strdup(node->path);
    char *path;

    if (startswith(path, "/disk0")) {
        path = &_path[6];
    } else {
        path = &_path[0];
    }

    RBFSNode *_node = find_file(path);

    if (_node) { // the file is already on disk, just adding the nodes to the vfs
        printf("PATH");
        free(_path);
        return 0;
    }

    int ret = rbfs_create_file(path, node->contents);
    free(_path);
    return ret;
}

int rbfs_vfs_mkdir(fs_node_t *node) {
    char *_path = strdup(node->path);
    char *path;

    if (startswith(path, "/disk0")) {
        path = &_path[6];
    } else {
        path = &_path[0];
    }

    int ret = rbfs_create_dir(path);
    free(_path);
    return ret;
}

void find_disk() {
    rbfs_disk = get_disk(0);
}

int init_rbfs() {
    find_disk();
    fs_node_t *node = mount_fs("disk0", "/", rbfs_vfs_write, rbfs_vfs_read, rbfs_vfs_mkfile, rbfs_vfs_mkdir, USER_PERMISSION);

    if (!node) {
        log::error("Failed to mount disk0");
    } else {
        log::info("Successfully mounted disk0");
        index_disk();
    }
    return 0;
}

void print_superblock_data() {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    rbfs_disk->read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    printf("Files: %d\n", super->files);
    printf("Magic: 0x%x\n", super->magic);
    printf("Status: %d\n", super->status);
    printf("Disk size: %d bytes\n", super->disk_size);
}
