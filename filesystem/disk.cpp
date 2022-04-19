#include "disk.h"
#include <drivers/disk/disk.h>
#include "utils.h"

void rbfs_format() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    RBFSSuperblock super;
    super.magic = RBFS_DISK_MAGIC;
    super.disk_size = rbfs_disk->total_sectors() * 512;
    super.status = RBFS_CLEAN;
    super.files = 0;
    super.first_free = RBFS_BEG + 1;

    rbfs_disk->write_one(RBFS_BEG, (uint8_t *)&super);
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

void rbfs_add_index(char *name, char *path, int type, int offset, int sectors) {
    RBFSIndex *index = new RBFSIndex();

    memset(index->name, 0, 20);
    memset(index->path, 0, 100);

    strcpy(index->name, name);
    strcpy(index->path, path);

    index->type = type;
    index->sector = offset;
    index->sectors = sectors;
    index->magic = RBFS_DISK_MAGIC;

    indexed[index_count] = index;
    index_count++;
}

void rbfs_index_disk() {
    uint8_t *_super = (uint8_t *)malloc(512);
    memset(_super, 0, 512);

    rbfs_disk->read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    const int max = super->first_free;
    int z = RBFS_BEG + 1;

    printf("Total files: %d\n", super->files);

    while (true) {
        if (z >= max) {
            break;
        }

        uint8_t *bytes = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);
        rbfs_disk->read(bytes, z, 1);

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

        rbfs_add_index(node->name, node->path, node->type, z, node->sectors);
        z += node->sectors;

        free(node);

        z++;
    }

    log::info("rbfs: rbfs_index_disk: successfully indexed %d files from disk", index_count);

    free(super);
}

void rescan() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    rbfs_index_disk();
}

void rbfs_find_disk() {
    rbfs_disk = get_disk(0);
}

int rbfs_add_node(char *path, int type, int perm, char *contents) {
    printf("A");
    if (rbfs_find_index(path)) {
        return 1;
    }

    printf("B");

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

    printf("C");

    node.uid = node.gid = 0;
    node.permission = perm;
    node.type = type;
    node.error = RBFS_CLEAN;
    node.magic = RBFS_DISK_MAGIC;
    node.sectors = strlen(contents)/512 + 1;

    printf("D");

    uint8_t _super[512];

    if (!_super) {
        return 1;
    }

    memset(_super, 0, 512);
    rbfs_disk->read(_super, RBFS_BEG, 1); // TODO <--- fix bug here

    printf("E");

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    if (super->magic != RBFS_DISK_MAGIC) {
        log::error("rbfs: add_node: superblock invalid magic");
        free(super);
        return 1;
    }

    printf("F");

    int offset = super->first_free;

    rbfs_disk->write_one(offset, (uint8_t *)&node);

    printf("G");

    if (contents) {
        int length = strlen(contents);
        uint8_t bytes[length + 1];
        memset(bytes, 0, length + 1);
        rbfs_ustr_from_str(bytes, contents, length);
        rbfs_disk->write(offset + 1, node.sectors, bytes);
    }

    printf("H");

    super->files++;
    super->first_free += 1;
    super->first_free += node.sectors;

    rbfs_disk->write_one(RBFS_BEG, (uint8_t *)super);

    printf("I");

    rbfs_add_index(node.name, node.path, node.type, offset, node.sectors);

    printf("J");

    free(super);

    return 0;
}

int rbfs_create_file(char *path, char *contents) {
    return rbfs_add_node(path, RBFS_FILE, 0, contents);
}

int rbfs_create_folder(char *path) {
    return rbfs_add_node(path, RBFS_DIR, 0, NULL);
}

RBFSIndex *rbfs_open(char *path) {
    return rbfs_find_index(path);
}

void rbfs_sector_up(int sector, bool do_rescan = false) {
    uint8_t *_super = (uint8_t *)malloc(512);
    memset(_super, 0, 512);

    rbfs_disk->read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    int max = super->first_free;

    for (int z = max - 1; z >= sector; z--) {
        uint8_t *bytes = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);

        rbfs_disk->read(bytes, z, 1);
        rbfs_disk->write_one(z + 1, bytes);

        free(bytes);
    }

    if (do_rescan) {
        rescan();
    }
}

void rbfs_sectors_up(int sector, int sectors) {
    for (int z = 0; z < sectors; z++) {
        rbfs_sector_up(sector);
    }

    rescan();
}

void rbfs_sector_down(int sector, bool do_rescan = false) {
    uint8_t *_super = (uint8_t *)malloc(512);
    memset(_super, 0, 512);

    rbfs_disk->read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    int max = super->first_free;

    for (int z = sector; z < max; z++) {
        uint8_t *bytes = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);

        rbfs_disk->read(bytes, z + 1, 1);
        rbfs_disk->write_one(z, bytes);

        free(bytes);
    }

    if (do_rescan) {
        rescan();
    }
}

void rbfs_sectors_down(int sector, int sectors) {
    for (int z = 0; z < sectors; z++) {
        rbfs_sector_down(sector);
    }

    rescan();
}

void rbfs_modify_file(RBFSIndex *index, char *contents) {
    int new_len = strlen(contents)/512 + 1;
    int offset = index->sector;
    char path[100];
    memset(path, 0, 100);
    strcpy(path, index->path);

    if (index->sectors > new_len) {
        int sects = index->sectors - new_len;
        rbfs_sectors_down(index->sector + 1, sects);
    } else if (index->sectors == new_len) {

    } else if (index->sectors < new_len) {
        int sects = new_len - index->sectors;
        rbfs_sectors_up(index->sector + 1, sects);
    }

    int length = strlen(contents);
    uint8_t *bytes = (uint8_t *)malloc(length + 1);
    memset(bytes, 0, length + 1);
    rbfs_ustr_from_str(bytes, contents, length);
    rbfs_disk->write(offset, new_len, bytes);

    index = rbfs_open(path);
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

    int ret = rbfs_disk->read(bytes, beg_sector, sectors);

    if (ret) {
        free(bytes);
        return 1;
    } else {
        rbfs_str_from_ustr(out, &bytes[offset], size);
        free(bytes);
        return 0;
    }
}

int rbfs_write(char *buf, int offset, int size, RBFSIndex *index) {
    char out[index->sectors * 512 + 1];
    memset(out, 0, index->sectors * 512 + 1);
    int ret = rbfs_read(out, 0, index->sectors * 512, index);

    if (ret) {
        return 1;
    }

    strncpy(&out[offset], buf, size);

    rbfs_modify_file(index, out);

    return 0;
}

void rbfs_init() {
    rbfs_find_disk();

    if (!rbfs_disk) {
        log::error("rbfs: rbfs_find_disk: failed to load disk");
    } else {
        log::info("rbfs: rbfs_find_disk: successfully loaded disk");
        rbfs_index_disk();
    }
}
