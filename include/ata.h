#pragma once

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define SECTOR_SIZE 512

void ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count);
void ata_write_one(uint32_t LBA, uint8_t *bytes);
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes);
