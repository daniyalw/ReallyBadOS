#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <tar.h>

bool Tar::parse(unsigned int address) {
    unsigned int i;

    for (i = 0; ; i++) {
        RawTar *header = (RawTar *)address;

        if (header->name[0] == '\0')
            break;

        u32 size = header->file_size();

        headers[i] = header;
        header_count++;

        address += ((size / 512) + 1) * 512;

        if (size % 512)
            address += 512;
    }

    if (!header_count) return false;

    for (int z = 0; z < header_count; z++) {
        TarFile block;

        //memcpy(headers[z].name, block.name, 100);
        block.name = headers[z]->name;
        block.size = headers[z]->file_size();
        block.typeflag = headers[z]->typeflag;

        for (int b = 0; b < 100; b++)
            block.linkname[z] = headers[z]->linkname[z];

        for (int b = 0; b < 8; b++) {
            block.uid[z] = headers[z]->uid[z];
            block.gid[z] = headers[z]->gid[z];
        }

        block.contents = (char *)headers[z] + 512;

        block.type = headers[z]->typeflag;

        blocks[z] = block;
        block_count++;
    }

    return true;
}

void __tar_test(int location) {
    Tar tar;

    bool r = tar.parse(location);

    printf("Returned: %s\nBlock count: %d\n", (char *)(r ? "True" : "False"), tar.block_count);

    for (int z = 0; z < tar.block_count; z++) {
        printf("Name: %s\nContents: %s\n", tar.blocks[z].name, tar.blocks[z].contents);
        log::warning("Name: %s", tar.blocks[z].name);
    }
}
