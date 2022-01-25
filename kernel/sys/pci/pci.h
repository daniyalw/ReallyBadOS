#pragma once

struct PCIDevice
{
    uint8_t interrupt;
    uint32_t base;

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

struct addr_reg
{
    bool prefetchable;
    int type;
    uint32_t size;
    uint64_t addr;
};

uint16_t read_pci(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
void write_pci(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg_offset, uint32_t value);
addr_reg get_addr_reg(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

bool device_functions(uint16_t bus, uint16_t device);
uint16_t get_class_id(uint16_t bus, uint16_t device, uint16_t function);
uint16_t get_subclass_id(uint16_t bus, uint16_t device, uint16_t function);

void scan_buses();
void go_through_and_print();
PCIDevice * find_device(uint16_t vendor, uint16_t device);

PCIDevice * devices[256*32*8];
int device_count = 0;
