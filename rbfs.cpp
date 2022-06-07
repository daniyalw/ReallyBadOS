#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "rbfs.h"

char disk_path[256];
FILE *disk = NULL;

bool strisempty(char *str) {
    if (!str[0]) {
        return true;
    }

    for (int z = 0; z < strlen(str); z++) {
        if (!isspace(str[z])) {
            return false;
        }
    }

    return true;
}

bool startswith(char * words, char * start) {
    int cl = strlen(start);
    int length = strlen(words);

    if (cl > length) {
        return false;
    }

    for (int z = 0; z < cl; z++) {
        if (words[z] != start[z]) {
            return false;
        }
    }

    return true;
}

char *find_parent(char *_path) {
    char *path = strdup(_path);

    if (strcmp(path, "/") == 0) {
        return path;
    }

    if (path[strlen(path) - 1] == '/') {
        path[strlen(path) - 1] = 0;
    }

    for (int z = strlen(path) - 1; z >= 0; z--) {
        if (path[z] == '/') {
            if (strcmp(path, "/")) {
                path[z] = 0;
            }
            break;
        }

        path[z] = 0;
    }

    return path;
}

char *find_name(char *path) {
    int last = 0;

    for (int z = 0; z < strlen(path); z++) {
        if (path[z] == '/') {
            last = z;
        }
    }

    char *name = strdup(path);
    memset(name, 0, strlen(path));
    int c = 0;

    for (int z = last + 1; z < strlen(path); z++) {
        name[c] = path[z];
        c++;
    }

    name[c] = 0;

    return name;
}

rbfs_time_t default_time() {
    rbfs_time_t _t;

    time_t s, val = 1;
    tm* t;
    s = time(NULL);
    t = localtime(&s);

    _t.day = t->tm_mday;
    _t.weekday = t->tm_wday;
    _t.month = t->tm_mon + 1;
    _t.year = t->tm_year + 1900;
    _t.hour = t->tm_hour;
    _t.min = t->tm_min;
    _t.sec = t->tm_sec;

    if (_t.hour >= 12) {
        _t.hour -= 12;
        _t.pm = true;
    } else {
        _t.pm = false;
    }

    return _t;
}

uint8_t *disk_read(uint8_t *target, uint32_t lba, int sector_count) {
    fseek(disk, lba * 512, SEEK_SET);
    fread((char *)target, 1, sector_count * 512, disk);

    return target;
}

void disk_write_one(uint32_t lba, uint8_t *bytes) {
    fseek(disk, lba * 512, SEEK_SET);
    fwrite((char *)bytes, 1, 512, disk);
}

void disk_write_one(uint32_t lba, RBFSNode *node) {
    fseek(disk, lba * 512, SEEK_SET);
    fwrite(node, sizeof(node), 1, disk);
}

void disk_write_one(uint32_t lba, RBFSSuperblock *node) {
    printf("Writing to disk, sector %d, byte location %d, to write superblock of size %lu\n", lba, lba * 512, sizeof(RBFSSuperblock));

    fseek(disk, lba * 512, SEEK_SET);
    fwrite(node, sizeof(RBFSSuperblock), 1, disk);
}

void disk_write(uint32_t lba, uint8_t sector_count, uint8_t *bytes) {
    fseek(disk, lba * 512, SEEK_SET);
    fwrite((char *)bytes, 1, sector_count * 512, disk);
}

void format() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;

    uint8_t b[512];
    disk_read(b, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)b;

    super->magic = RBFS_DISK_MAGIC;
    super->first_free = RBFS_BEG + 1;
    super->disk_size = 15 * 1048576;
    super->status = 0;
    super->files = 0;

    disk_write_one(RBFS_BEG, (uint8_t *)super);
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
        printf("Index: \n\tPath: %s\n\tSector: %d\n", indexed[z]->path, indexed[z]->sector);
    }
}

void cleanup_indexes() {
    for (int z = 0; z < index_count; z++) {
        free(indexed[z]);
    }

    index_count = 0;
}

void list_dir(char *path) {
    for (int z = 0; z < index_count; z++) {
        if (startswith(indexed[z]->path, path)) {
            printf("%s     (%s)\n", indexed[z]->path, (char *)(indexed[z]->type ? "file" : "directory"));
        }
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

    memcpy(index, node, sizeof(RBFSNode));
    index->sector = offset;
    index->id = index_count;

    indexed[index_count] = index;
    index_count++;
}

void add_index(RBFSNode node, int offset) {
    RBFSIndex *index = new RBFSIndex();

    memcpy(index, &node, sizeof(RBFSNode));
    index->sector = offset;
    index->id = index_count;

    indexed[index_count] = index;
    index_count++;
}

void index_disk() {
    uint8_t *_super = (uint8_t *)malloc(512);
    memset(_super, 0, 512);

    disk_read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    const int max = super->first_free;
    int z = RBFS_BEG + 1;

    while (true) {
        if (z >= max) {
            break;
        }

        uint8_t *bytes = (uint8_t *)malloc(512);
        memset(bytes, 0, 512);
        disk_read(bytes, z, 1);

        RBFSNode *node = (RBFSNode *)bytes;

        if (node->magic != RBFS_DISK_MAGIC) {
            free(node);
            z++;
            continue;
        }

        if (strisempty(node->name)) {
            free(node);
            z++;
            continue;
        }

        add_index(node, z);

        z += node->sectors;

        free(node);

        z++;
    }

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

    RBFSNode *node = new RBFSNode();

    memset(node->name, 0, 20);
    memset(node->path, 0, 20);

    if (strcmp(path, "/")) {
        char *name = find_name(path);

        strcpy(node->name, name);
        strcpy(node->path, path);

        free(name);
    } else {
        strcpy(node->name, "/");
        strcpy(node->path, "/");
    }

    node->uid = node->gid = 0;
    node->permission = perm;
    node->type = type;
    node->error = RBFS_CLEAN;
    node->magic = RBFS_DISK_MAGIC;
    node->sectors = strlen(contents)/512 + 1;
    node->ctime = default_time();

    uint8_t _super[512];
    memset(_super, 0, 512);
    disk_read(_super, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)_super;

    if (super->magic != RBFS_DISK_MAGIC) {
        printf("Disk is not formatted correctly.\n");
        return 1;
    }

    int offset = super->first_free;

    disk_write_one(offset, (uint8_t *)node);

    if (contents) {
        int length = strlen(contents);
        uint8_t bytes[length + 1];
        memset(bytes, 0, length + 1);
        ustr_from_str(bytes, contents, length);
        disk_write(offset + 1, node->sectors, bytes);
    }

    super->files++;
    super->first_free += 1;
    super->first_free += node->sectors;

    disk_write_one(RBFS_BEG, (uint8_t *)super);
    add_index(node, offset);

    delete node;

    return 0;
}

int create_file(char *path, char *contents) {
    return add_node(path, RBFS_FILE, 0, contents);
}

int create_folder(char *path) {
    return add_node(path, RBFS_DIR, 0, "");
}

int create_file_auth(char *path, char *contents) {
    return add_node(path, RBFS_FILE, 1, contents);
}

RBFSIndex *rbfs_open(char *path) {
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
    disk_read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = super->first_free - 1; z >= sector; z--) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        disk_read(b, z, 1);
        disk_write_one(z + 1, b);

        free(b);
    }

    super->first_free++;
    disk_write_one(RBFS_BEG, (uint8_t *)super);

    free(super);
}

// sector can be overwritten
void move_sector_down(int sector) {
    uint8_t *bytes = (uint8_t *)malloc(512);
    memset(bytes, 0, 512);
    disk_read(bytes, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)bytes;

    for (int z = sector; z <= super->first_free; z++) {
        uint8_t *b = (uint8_t *)malloc(512);
        memset(b, 0, 512);

        disk_read(b, z + 1, 1);
        disk_write_one(z, b);

        free(b);
    }

    super->first_free--;
    disk_write_one(RBFS_BEG, (uint8_t *)super);

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
    disk_read(bytes, index->sector, 1);

    RBFSNode *node = (RBFSNode *)bytes;

    node->sectors = sectors;

    uint8_t *c = (uint8_t *)malloc(512);
    memset(c, 0, 512);
    disk_read(c, RBFS_BEG, 1);

    RBFSSuperblock *super = (RBFSSuperblock *)c;

    super->first_free += index->sectors - sectors;

    disk_write(index->sector + 1, sectors, b);
    disk_write_one(index->sector, (uint8_t *)node);
    disk_write_one(RBFS_BEG, (uint8_t *)super);

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

    for (int z = index->id; z < index_count; z++) {
        if (z == (index_count - 1)) {
            break;
        }

        indexed[z] = indexed[z + 1];
    }

    index_count--;

    for (int z = 0; z < index_count; z++) {
        auto i = indexed[z];
        i->id--;
        indexed[z] = i;
    }

    delete index;
}

int rbfs_read(char *out, int offset, int size, RBFSIndex *index) {
    int beg_sector = index->sector + 1;
    int sectors = index->sectors;

    uint8_t *bytes = (uint8_t *)malloc(size + 1);
    memset(bytes, 0, size + 1);

    disk_read(bytes, beg_sector, sectors);

    str_from_ustr(out, &bytes[offset], size);
    free(bytes);
    return 0;
}

int rbfs_write(char *buf, int offset, int size, RBFSIndex *index) {
    modify_file(index, buf);

    return 0;
}

void show_usage() {
    printf("usage: rbfs <option> <filename>\n");
}

void cleanup() {
    cleanup_indexes();
    fclose(disk);
}

int main(int argc, char *argv[]) {
    memset(disk_path, 0, 256);
    strcpy(disk_path, "out.img");

    disk = fopen(disk_path, "rb+");

    if (!disk) {
        fprintf(stderr, "error: failed to find disk.\n");
        return 1;
    }

    if (argc == 1) {
        show_usage();
        return 1;
    }

    index_disk();

    char *op = argv[1];

    if (strcmp(op, "-f") == 0 || strcmp(op, "--format") == 0) {
        format();
        printf("Formatted disk.\n");
    } else if (strcmp(op, "-r") == 0 || strcmp(op, "--read") == 0) {
        char *fname = argv[2];
        char out[100];
        memset(out, 0, 100);

        auto node = rbfs_open(fname);

        if (!node) {
            printf("Failed to open file.\n");
            cleanup();
            return 1;
        }

        int ret = rbfs_read(out, 0, 100, node);

        if (ret) {
            printf("Failed to read file.\n");
            cleanup();
            return 1;
        }

        printf("%s\n", out);
    } else if (strcmp(op, "-l") == 0 || strcmp(op, "--list") == 0) {
        if (argc > 2) {
            list_dir(argv[2]);
        } else {
            list_dir("/");
        }
    } else if (strcmp(op, "-w") == 0 || strcmp(op, "--write") == 0) {
        auto node = rbfs_open(argv[2]);

        if (!node) {
            printf("Failed to find file.\n");
            cleanup();
            return 1;
        }

        int len = 0;

        for (int z = 3; z < argc; z++) {
            len += strlen(argv[z]);
            len++;
        }

        char contents[len];
        memset(contents, 0, len);

        for (int z = 3; z < argc; z++) {
            strcat(contents, argv[z]);
            strcat(contents, " ");
        }

        int ret = rbfs_write(contents, 0, len, node);

        if (ret) {
            printf("Failed to write to file.\n");
            cleanup();
            return 1;
        }

        printf("Wrote to file.\n");
    } else if (strcmp(op, "-c") == 0 || strcmp(op, "--create") == 0) {
        // TODO fix
        if (argc <= 3) {
            printf("Not enough parameters.\n");
            cleanup();
            return 1;
        }

        int ret = 2;

        if (strcmp(argv[2], "folder") == 0) {
            ret = create_folder(argv[3]);
        } else if (strcmp(argv[2], "file") == 0) {
            ret = create_file(argv[3], "default");
        }

        switch (ret) {
            case 0:
                printf("Creation of '%s' successful.\n", argv[3]);
                break;

            case 1:
                printf("Failed to create '%s'.\n", argv[3]);
                break;

            default:
                printf("Unknown error occured.\n");
                break;
        }
    } else if (strcmp(op, "-i") == 0 || strcmp(op, "--info")) {
        if (argc < 3) {
            printf("Not enough parameters.\n");
            cleanup();
            return 1;
        }

        auto node = rbfs_open(argv[2]);

        if (!node) {
            printf("File to stat not found.\n");
            cleanup();
            return 1;
        }

        char * weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        char * months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

        printf("Name: %s\n", node->name);
        printf("Path: %s\n", node->path);
        printf("Size on disk: %d bytes (%d KB)\n", node->length, node->length/1024);
        printf("Type of file: %s\n", (char *)(node->type ? "file" : "directory"));
        printf("Creation time: ");
        printf("%s, %s %d ", weekdays[node->ctime.weekday-1], months[node->ctime.month-1], node->ctime.day);

        if (node->ctime.min < 10)
            printf("%d:0%d %s\n", node->ctime.hour, node->ctime.min, (char *)(node->ctime.pm ? "PM" : "AM"));
        else
            printf("%d:%d %s\n", node->ctime.hour, node->ctime.min, (char *)(node->ctime.pm ? "PM" : "AM"));

        if (strcmp(argv[2], "/") == 0) {
            printf("\nDisk data: \n");
            // print extra disk data
            uint8_t *_super = (uint8_t *)malloc(512);
            memset(_super, 0, 512);
            disk_read(_super, RBFS_BEG, 1);

            RBFSSuperblock *super = (RBFSSuperblock *)_super;

            printf("Total files: %d\n", super->files);
            printf("Total disk space: %d bytes (%d MB)\n", super->disk_size, super->disk_size/(1024 * 1024));
            printf("Total used space: %d bytes (%d MB)\n", super->first_free * 512, (super->first_free * 512)/(1024 * 1024));
            printf("Total free space: %d bytes (%d MB)\n", (super->disk_size - super->first_free * 512), (super->disk_size - super->first_free * 512)/(1024 * 1024));

            free(super);
        }
    } else if (strcmp(op, "-d") == 0 || strcmp(op, "--delete") == 0) {
        if (argc < 3) {
            printf("Not enough parameters.\n");
            cleanup();
            return 1;
        }

        auto node = rbfs_open(argv[2]);

        if (!node) {
            printf("Failed to find file.\n");
            cleanup();
            return 1;
        }

        delete_node(node);

        printf("Deleted node.\n");
    }

    cleanup();

    return 0;
}
