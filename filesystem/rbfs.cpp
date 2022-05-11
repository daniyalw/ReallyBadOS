#include "rbfs.h"
#include <drivers/disk/disk.h>
#include "utils.h"

namespace rbfs {

void format() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    uint8_t b[512];
    DiskDrivers::ATA::ata_read(b, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)b;

    super->magic = RBFS_DISK_MAGIC;
    super->first_free = RBFS_BEG + 1;
    super->disk_size = DiskDrivers::ATA::total_bytes();
    super->status = 0;
    super->files = 0;

    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);
}

RBFSIndex *find_index(char *path) {
    for (int z = 0; z < index_count; z++) {
        if (strcmp(indexed[z]->path, path) == 0) {
            return indexed[z];
        }
    }

    return NULL;
}

void print_indexes() {
    for (int z = 0; z < index_count; z++) {
        Kernel::serial_write_string("Index: \n\tPath: %s\n\tSector: %d\n", indexed[z]->path, indexed[z]->sector);
    }
}

void str_from_ustr(char *str, uint8_t *ustr, int size) {
    for (int z = 0; z < size; z++) {
        str[z] = (char)ustr[z];
    }
}

void ustr_from_str(uint8_t *out, char *str, int size) {
    for (int z = 0; z < size; z++) {
        out[z] = (uint8_t)str[z];
    }
}

void add_index(RBFSNode *node, int offset) {
    RBFSIndex *index = new RBFSIndex();

    memset(index->name, 0, 20);
    memset(index->path, 0, PATH_LIMIT);

    strcpy(index->name, node->name);
    strcpy(index->path, node->path);

    index->type = node->type;
    index->sector = offset;
    index->sectors = node->sectors;
    index->magic = RBFS_DISK_MAGIC;
    index->id = index_count;
    index->permission = node->permission;
    index->ctime = node->ctime;

    indexed[index_count] = index;
    index_count++;
}

void add_index(RBFSNode node, int offset) {
    RBFSIndex *index = new RBFSIndex();

    memset(index->name, 0, 20);
    memset(index->path, 0, PATH_LIMIT);

    strcpy(index->name, node.name);
    strcpy(index->path, node.path);

    index->type = node.type;
    index->sector = offset;
    index->sectors = node.sectors;
    index->magic = RBFS_DISK_MAGIC;
    index->id = index_count;
    index->permission = node.permission;
    index->ctime = node.ctime;

    indexed[index_count] = index;
    index_count++;
}

void index_disk() {
    uint8_t *_super = (uint8_t *)malloc(512);
    memset(_super, 0, 512);

    DiskDrivers::ATA::ata_read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    const int max = super->first_free;
    int z = RBFS_BEG + 1;

    while (true) {
        if (z >= max) {
            break;
        }

        uint8_t *bytes = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);
        DiskDrivers::ATA::ata_read(bytes, z, 1);

        RBFSNode *node = (RBFSNode *)bytes;

        if (node->magic != RBFS_DISK_MAGIC) {
            free(node);
            z++;
            continue;
        }

        if (!node->name || strisempty(node->name)) {
            free(node);
            z++;
            continue;
        }

        add_index(node, z);

        char *out = (char *)malloc(PATH_LIMIT);
        memset(out, 0, PATH_LIMIT);
        char *parent = find_parent(node->path);

        if (strcmp(parent, "/") == 0) {
            sprintf(out, "/disk0");
        } else {
            sprintf(out, "/disk0%s", parent);
        }

        if (node->type == RBFS_FILE) {
            create_file(node->name, out);
        } else if (node->type == RBFS_DIR) {
            make_dir(node->name, out);
        }

        z += node->sectors;

        free(node);

        z++;
    }

    log::info("rbfs: %s: successfully indexed %d files from disk", __FUNCTION__, index_count);

    free(super);
}

void rescan() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    index_disk();
}

int add_node(char *path, int type, int perm, char *contents) {
    if (find_index(path)) {
        return 1;
    }

    if (strcmp(path, "/")) {
        char *parent = find_parent(path);

        if (!find_index(parent)) {
            free(parent);
            return 1;
        }

        free(parent);
    }

    RBFSNode node;

    memset(node.name, 0, 20);
    memset(node.path, 0, 20);

    if (strcmp(path, "/")) {
        char *name = find_name(path);

        strcpy(node.name, name);
        strcpy(node.path, path);

        free(name);
    } else {
        strcpy(node.name, "/");
        strcpy(node.path, "/");
    }

    node.uid = node.gid = 0;
    node.permission = perm;
    node.type = type;
    node.error = RBFS_CLEAN;
    node.magic = RBFS_DISK_MAGIC;
    node.sectors = strlen(contents)/512 + 1;
    node.ctime = Time::get_time();

    uint8_t _super[512];
    memset(_super, 0, 512);
    DiskDrivers::ATA::ata_read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    if (super->magic != RBFS_DISK_MAGIC) {
        log::error("rbfs: %s: superblock invalid magic", __FUNCTION__);
        free(super);
        return 1;
    }

    int offset = super->first_free;

    DiskDrivers::ATA::ata_write_one(offset, (uint8_t *)&node);

    if (contents) {
        int length = strlen(contents);
        uint8_t bytes[length + 1];
        memset(bytes, 0, length + 1);
        ustr_from_str(bytes, contents, length);
        DiskDrivers::ATA::ata_write(offset + 1, node.sectors, bytes);
    }

    super->files++;
    super->first_free += 1;
    super->first_free += node.sectors;

    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);
    add_index(node, offset);

    free(super);

    return 0;
}

int create_file(char *path, char *contents) {
    return add_node(path, RBFS_FILE, 0, contents);
}

int create_folder(char *path) {
    return add_node(path, RBFS_DIR, 0, NULL);
}

int create_file_auth(char *path, char *contents) {
    return add_node(path, RBFS_FILE, 1, contents);
}

RBFSIndex *open(char *path) {
    auto index = find_index(path);

    if (index) {
        if (index->permission >= RBFS_PERM_ROOT) {
            return NULL;
        }

        return index;
    }

    return NULL;
}

RBFSIndex *open_root(char *path) {
    auto index = find_index(path);

    if (index) {
        if (index->permission > RBFS_PERM_ROOT) {
            return NULL;
        }

        return index;
    }

    return NULL;
}

void move_sector_up(int sector) {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    DiskDrivers::ATA::ata_read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = super->first_free - 1; z >= sector; z--) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        DiskDrivers::ATA::ata_read(b, z, 1);
        DiskDrivers::ATA::ata_write_one(z + 1, b);

        free(b);
    }

    super->first_free++;
    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);

    free(super);
}

// sector can be overwritten
void move_sector_down(int sector) {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    DiskDrivers::ATA::ata_read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = sector; z <= super->first_free; z++) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        DiskDrivers::ATA::ata_read(b, z + 1, 1);
        DiskDrivers::ATA::ata_write_one(z, b);

        free(b);
    }

    super->first_free--;
    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);

    free(super);
}

void sectors_up(int sector, int sectors) {
    for (int z = 0; z < sectors; z++)
        move_sector_up(sector + z);
}

void sectors_down(int sector, int sectors) {
    for (int z = sectors; z >= 0; z--)
        move_sector_down(sector + z);
}

void modify_file(RBFSIndex *index, char *contents) {
    int sectors = strlen(contents)/512 + 1;
    int contents_beg = index->sector + 1;

    if (sectors < index->sectors)
        sectors_down(contents_beg + (index->sectors - sectors), index->sectors - sectors);
    else if (sectors > index->sectors)
        sectors_up(contents_beg + index->sectors, sectors - index->sectors);

    index->sectors = sectors;

    int _len = strlen(contents);
    uint8_t *b = (uint8_t *)malloc(_len + 1);
    memset(b, 0, _len + 1);
    ustr_from_str(b, contents, _len);

    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    DiskDrivers::ATA::ata_read(bytes, index->sector, 1);

    RBFSNode *node = (RBFSNode *)bytes;

    node->sectors = sectors;

    uint8_t *c = (uint8_t *)malloc(512);
    memset(c, 0, 512);
    DiskDrivers::ATA::ata_read(c, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)c;

    super->first_free += index->sectors - sectors;

    DiskDrivers::ATA::ata_write(index->sector + 1, sectors, b);
    DiskDrivers::ATA::ata_write_one(index->sector, (uint8_t *)node);
    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);

    free(b);
    free(bytes);
    free(super);

    for (int z = index->id + 1; z < index_count; z++) {
        auto _in = indexed[z];

        _in->sector -= index->sectors - sectors;
    }

    return;
}

void delete_node(RBFSIndex *index) {
    int offset = index->sector;
    int sectors = index->sectors;
    sectors_down(offset, sectors + 1);
}

int read(char *out, int offset, int size, RBFSIndex *index) {
    int beg_sector = index->sector + 1;
    int sectors = index->sectors;

    uint8_t *bytes = (uint8_t *)malloc(size + 1);
    memset(bytes, 0, size + 1);

    DiskDrivers::ATA::ata_read(bytes, beg_sector, sectors);

    str_from_ustr(out, &bytes[offset], size);
    free(bytes);
    return 0;
}

int write(char *buf, int offset, int size, RBFSIndex *index) {
    modify_file(index, buf);

    return 0;
}

// ---------------- VFS functions --------- //
int vfs_write(fs_node_t *node, int offset, int size, char *buf) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];
    auto index = find_index(path);

    if (!index) {
        return 1;
    }

    free(orig);

    return write(buf, offset, size, index);
}

int vfs_read(fs_node_t *node, int offset, int size, char *buf) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];
    auto index = find_index(path);

    if (!index) {
        return 1;
    }

    free(orig);

    return read(buf, offset, size, index);
}

int vfs_mkfile(fs_node_t *node) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];

    // verify that the file doesn't exist
    auto index = open(path);

    if (index) {
        node->ctime = index->ctime;
        // the file is on disk, just need to tell vfs
        return 0;
    }

    int ret = create_file(path, " ");
    free(orig);
    return ret;
}

int vfs_mkdir(fs_node_t *node) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];

    // verify that the file doesn't exist
    auto index = open(path);

    if (index) {
        // the file is on disk, just need to tell vfs
        return 0;
    }

    int ret = create_folder(path);
    free(orig);
    return ret;
}

fs_driver_t vfs_driver = {
    NULL,
    NULL,
    vfs_write,
    vfs_read,
    vfs_mkfile,
    vfs_mkdir
};

// --------------- init ----------------- //
void init() {
    auto node = mount_fs("disk0", "/", vfs_driver, USER_PERMISSION);

    if (node) {
        log::info("rbfs: %s: successfully mounted disk", __FUNCTION__);
        index_disk();
    } else {
        log::error("rbfs: %s: failed to mount disk", __FUNCTION__);
    }
}

}
