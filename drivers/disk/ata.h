#pragma once

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define SECTOR_SIZE 512

#define DRIVE_MASTER 0xA0
#define DRIVE_SLAVE 0xB0

#define OFFSET_MODEL 27
#define OFFSET_MODEL_SERIAL 176

namespace DiskDrivers {

namespace ATA {

uint16_t *ata_init(uint16_t *bytes);

uint16_t * ata_send_identify(uint16_t *bytes);

uint8_t *ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count);
void ata_write_one(uint32_t LBA, uint8_t *bytes);
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes);

void select_drive();

uint32_t total_sectors();
uint32_t total_bytes();

}

}
