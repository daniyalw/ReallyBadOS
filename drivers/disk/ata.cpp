#include <disk/ata.h>
#include <string.h>

using namespace std;

// target_address = your variable to where to store
// lba = which sector
// sector_count = how many sectors to read
void ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count) {
    while (inb(0x1F7) & STATUS_BSY)
        ;
    outb(0x1F1, 0x00);
    outb(0x1F2, sector_count);
    outb(0x1F3, (uint8_t)LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
    outb(0x1F5, (uint8_t)(LBA >> 16));
    outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    outb(0x1F7, 0x20);

    uint8_t *target = target_address;

    for (int j = 0; j < sector_count; j++) {
        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint16_t tmpword = inw(0x1F0);
            target[i * 2] = tmpword;
            target[(i * 2) + 1] = tmpword >> 8;
        }

        target += 256;
    }
}

void ata_write_one(uint32_t LBA, uint8_t *bytes) {
    while (inb(0x1F7) & STATUS_BSY)
        ;
    outb(0x1F1, 0x00);
    outb(0x1F2, 0x01);
    outb(0x1F3, (uint8_t)LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
    outb(0x1F5, (uint8_t)(LBA >> 16));
    outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    outb(0x1F7, 0x30);

    for (int j = 0; j < 1; j++) {
        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint32_t *re = (uint32_t *)*(uint32_t *)&bytes;
            outl(0x1F0, re[i]);
        }
    }
}

// Calculate sectors needed by a file: "(file size in bytes / bytes by sector) +
// 1"
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes) {
    for (int i = 0; i < sector_count; i++) {
        ata_write_one(LBA + i, bytes);
        bytes += SECTOR_SIZE;
    }
}

void fs_ata_write(char * str)
{
    fs_ata_t ata = get_from_str(str);

    ata_write_one(ata.lba, ata.bytes);
}

char * fs_ata_read(char * num)
{
    char * data = (char *)malloc(SECTOR_SIZE);
    uint32_t sector = (uint32_t)atoi(num);
    uint8_t *bytes;

    ata_read(bytes, sector, 1);

    for (int z = 0; z < SECTOR_SIZE; z++) data[z] = (char)bytes[z];

    return data;
}

uint8_t * ata_init(uint8_t *bytes) {
    uint8_t abc = inb(0x1F5);
    bool non_packet = false;

    if (abc == 0xEB) {
        non_packet = true;
    }

    if (!non_packet) {
        outb(0x1F6, 0xA0);
        outb(0x1F2, 0);
        outb(0x1F3, 0);
        outb(0x1F4, 0);
        outb(0x1F5, 0);
        outb(0x1F7, 0xEC);
        uint8_t res = inb(0x1F7);

        if (res == 0) {
            printf("Drive does not exist!\n");
            return NULL;
        }

        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = inw(0x1F0);
        }

        create_file("ata", "dev", fs_ata_read, fs_ata_write);

        return bytes;
    } else {
        outb(0x1F6, 0xA1);
        outb(0x1F2, 0);
        outb(0x1F3, 0);
        outb(0x1F4, 0);
        outb(0x1F5, 0);
        outb(0x1F7, 0xEC);
        uint8_t res = inb(0x1F7);

        if (res == 0) {
            printf("Non-existent drive!\n");
            return NULL;
        }

        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = inw(0x1F0);
        }

        create_file("ata", "dev", fs_ata_read, fs_ata_write);

        return bytes;
    }
}

FILE * read_file_from_disk(uint32_t LBA, uint32_t sectors)
{
    FILE * file;
    uint8_t * bytes;

    ata_read(bytes, LBA, sectors);

    file = (FILE *)bytes;

    return file;
}

fs_ata_t get_from_str(char * str)
{
    uint8_t bytes[strlen(str)];
    int bz = 0;
    uint32_t lba;
    fs_ata_t ata;

    char slba[strlen(str)];
    int sz = 0;

    for (int z = 0; z < strlen(str); z++)
    {
        if (str[z] == ':')
        {
            break;
        }
        else
        {
            slba[sz] = str[z];
            sz++;
        }
    }

    slba[sz] = 0;

    for (int z = sz+1; z < strlen(str); z++)
    {
        bytes[bz] = (uint8_t)str[z];
        bz++;
    }

    bytes[bz] = 0;

    lba = (uint32_t)atoi(slba);

    ata.bytes = bytes;
    ata.lba = lba;

    return ata;
}
