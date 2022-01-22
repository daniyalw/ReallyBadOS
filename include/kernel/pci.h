#pragma once

struct PCIDevice
{
    uint8_t interrupt;
    uint32_t portBase;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t deviceID;
    uint16_t vendorID;

    uint8_t classID;
    uint8_t subclassID;
    uint8_t interfaceID;
    uint8_t revisionID;
} __attribute__((packed));

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
void write_pci();

PCIDevice * devices[256*32*8];
int device_count = 0;
