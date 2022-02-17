#pragma once

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define SECTOR_SIZE 512

typedef struct
{
    uint32_t lba;
    uint8_t *bytes;
} fs_ata_t;

uint16_t * ata_init(uint16_t *bytes);

void ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count);
void ata_write_one(uint32_t LBA, uint8_t *bytes);
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes);

FILE * read_file_from_disk(uint32_t LBA, uint32_t sectors);
fs_ata_t get_from_str(char * str);

uint32_t total_sectors();
uint32_t total_bytes();
