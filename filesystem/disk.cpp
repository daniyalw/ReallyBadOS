#include "disk.h"
#include <drivers/disk/disk.h>
#include "utils.h"

void rbfs_format() {
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

RBFSIndex *rbfs_find_index(char *path) {
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

void rbfs_str_from_ustr(char *str, uint8_t *ustr, int size) {
    for (int z = 0; z < size; z++) {
        str[z] = (char)ustr[z];
    }
}

void rbfs_ustr_from_str(uint8_t *out, char *str, int size) {
    for (int z = 0; z < size; z++) {
        out[z] = (uint8_t)str[z];
    }
}

void rbfs_add_index(char *name, char *path, int type, int offset, int sectors, int perm) {
    RBFSIndex *index = new RBFSIndex();

    memset(index->name, 0, 20);
    memset(index->path, 0, 100);

    strcpy(index->name, name);
    strcpy(index->path, path);

    index->type = type;
    index->sector = offset;
    index->sectors = sectors;
    index->magic = RBFS_DISK_MAGIC;
    index->id = index_count;
    index->permission = perm;

    indexed[index_count] = index;
    index_count++;
}

void rbfs_index_disk() {
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

        rbfs_add_index(node->name, node->path, node->type, z, node->sectors, node->permission);

        char *out = (char *)malloc(100);
        memset(out, 0, 100);
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

void rbfs_rescan() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    rbfs_index_disk();
}

int rbfs_add_node(char *path, int type, int perm, char *contents) {
    if (rbfs_find_index(path)) {
        return 1;
    }

    if (strcmp(path, "/")) {
        char *parent = find_parent(path);

        if (!rbfs_find_index(parent)) {
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

    uint8_t _super[512];
    memset(_super, 0, 512);
    DiskDrivers::ATA::ata_read(_super, RBFS_BEG, 1); // TODO <--- fix bug here

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
        rbfs_ustr_from_str(bytes, contents, length);
        DiskDrivers::ATA::ata_write(offset + 1, node.sectors, bytes);
    }

    super->files++;
    super->first_free += 1;
    super->first_free += node.sectors;

    DiskDrivers::ATA::ata_write_one(RBFS_BEG, (uint8_t *)super);
    rbfs_add_index(node.name, node.path, node.type, offset, node.sectors, node.permission);

    free(super);

    return 0;
}

int rbfs_create_file(char *path, char *contents) {
    return rbfs_add_node(path, RBFS_FILE, 0, contents);
}

int rbfs_create_folder(char *path) {
    return rbfs_add_node(path, RBFS_DIR, 0, NULL);
}

int rbfs_create_file_auth(char *path, char *contents) {
    return rbfs_add_node(path, RBFS_FILE, 1, contents);
}

RBFSIndex *rbfs_open(char *path) {
    auto index = rbfs_find_index(path);

    if (index) {
        if (index->permission >= RBFS_PERM_ROOT) {
            return NULL;
        }

        return index;
    }

    return NULL;
}

RBFSIndex *rbfs_open_root(char *path) {
    auto index = rbfs_find_index(path);

    if (index) {
        if (index->permission > RBFS_PERM_ROOT) {
            return NULL;
        }

        return index;
    }

    return NULL;
}

void rbfs_move_sector_up(int sector) {
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
void rbfs_move_sector_down(int sector) {
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

void rbfs_sectors_up(int sector, int sectors) {
    for (int z = 0; z < sectors; z++)
        rbfs_move_sector_up(sector + z);
}

void rbfs_sectors_down(int sector, int sectors) {
    for (int z = sectors; z >= 0; z--)
        rbfs_move_sector_down(sector + z);
}

void rbfs_modify_file(RBFSIndex *index, char *contents) {
    int sectors = strlen(contents)/512 + 1;
    int contents_beg = index->sector + 1;

    if (sectors < index->sectors)
        rbfs_sectors_down(contents_beg + (index->sectors - sectors), index->sectors - sectors);
    else if (sectors > index->sectors)
        rbfs_sectors_up(contents_beg + index->sectors, sectors - index->sectors);

    index->sectors = sectors;

    int _len = strlen(contents);
    uint8_t *b = (uint8_t *)malloc(_len + 1);
    memset(b, 0, _len + 1);
    rbfs_ustr_from_str(b, contents, _len);

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

void rbfs_delete_node(RBFSIndex *index) {
    int offset = index->sector;
    int sectors = index->sectors;
    rbfs_sectors_down(offset, sectors + 1);
}

int rbfs_read(char *out, int offset, int size, RBFSIndex *index) {
    int beg_sector = index->sector + 1;
    int sectors = index->sectors;

    uint8_t *bytes = (uint8_t *)malloc(size + 1);
    memset(bytes, 0, size + 1);

    DiskDrivers::ATA::ata_read(bytes, beg_sector, sectors);

    rbfs_str_from_ustr(out, &bytes[offset], size);
    free(bytes);
    return 0;
}

int rbfs_write(char *buf, int offset, int size, RBFSIndex *index) {
    rbfs_modify_file(index, buf);

    return 0;
}

// ---------------- VFS functions --------- //
int rbfs_vfs_write(fs_node_t *node, int offset, int size, char *buf) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];
    auto index = rbfs_find_index(path);

    if (!index) {
        return 1;
    }

    free(orig);

    return rbfs_write(buf, offset, size, index);
}

int rbfs_vfs_read(fs_node_t *node, int offset, int size, char *buf) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];
    auto index = rbfs_find_index(path);

    if (!index) {
        return 1;
    }

    free(orig);

    return rbfs_read(buf, offset, size, index);
}

int rbfs_vfs_mkfile(fs_node_t *node) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];

    // verify that the file doesn't exist
    auto index = rbfs_open(path);

    if (index) {
        // the file is on disk, just need to tell vfs
        return 0;
    }

    int ret = rbfs_create_file(path, " ");
    free(orig);
    return ret;
}

int rbfs_vfs_mkdir(fs_node_t *node) {
    char *orig = strdup(node->path);
    char *path = &orig[strlen(RBFS_MOUNT)];

    // verify that the file doesn't exist
    auto index = rbfs_open(path);

    if (index) {
        // the file is on disk, just need to tell vfs
        return 0;
    }

    int ret = rbfs_create_folder(path);
    free(orig);
    return ret;
}

// --------------- init ----------------- //
void rbfs_init() {
    auto node = mount_fs("disk0", "/", rbfs_vfs_write, rbfs_vfs_read, rbfs_vfs_mkfile, rbfs_vfs_mkdir, USER_PERMISSION);

    if (node) {
        log::info("rbfs: %s: successfully mounted disk", __FUNCTION__);
        rbfs_index_disk();
    } else {
        log::error("rbfs: %s: failed to mount disk", __FUNCTION__);
    }
}
