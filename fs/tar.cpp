#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <tar.h>

bool parse(unsigned int address)
{
    unsigned int i;

    for (i = 0; ; i++)
    {
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

    for (int z = 0; z < header_count; z++)
    {
        TarFile block;

        //std::memcpy(headers[z].name, block.name, 100);
        block.name = headers[z]->name;
        block.size = headers[z]->file_size();
        block.typeflag = headers[z]->typeflag;
        std::memcpy(block.linkname, headers[z]->linkname, 100);
        block.contents = (char *)headers[z] + 512;

        blocks[z] = block;
        block_count++;
    }

    return true;
}

void __tar_test(int location)
{
    bool r = parse(location);

    printf("Returned: %s\nBlock count: %d\n", (char *)(r ? "True" : "False"), block_count);

    for (int z = 0; z < block_count; z++)
    {
        printf("Name: %s\nContents: %s\n", blocks[z].name, blocks[z].contents);
        Kernel::system_log("Name: %s\n", blocks[z].name);
    }
}
