#pragma once

namespace Kernel {

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

struct ClassType {
    uint16_t id;
    char *name;
};

ClassType class_names[] = {
    {0x0, "Unclassified"},
    {0x1, "Mass Storage Controller"},
    {0x2, "Network Controller"},
    {0x3, "Display Controller"},
    {0x4, "Multimedia Controller"},
    {0x5, "Memory Controller"},
    {0x6, "Bridge"},
    {0x7, "Simple Communication Controller"},
    {0x8, "Base System Peripheral"},
    {0x9, "Input Device Controller"},
    {0xA, "Docking Station"},
    {0xB, "Processor"},
    {0xC, "Serial Bus Controller"},
    {0xD, "Wireless Controller"},
    {0xE, "Intelligent Controller"},
    {0xF, "Satellite Communication Controller"},
    {0x10, "Encryption Controller"},
    {0x11, "Signal Processing Controller"},
    {0x12, "Processing Accelerator"},
    {0x13, "Non-essential instrumentation"},
    {0x14, "Reserved"},
    {0x40, "Co-processor"},
    {0x41, "Reserved"},
    {0x0, NULL}
};

int class_name_count = 0;

struct SubclassType {
    uint16_t cid;
    uint16_t sid;
    char *name;
} sub_names[] = {
    /* Unclassified */
    {0x0, 0x0, "Non-VGA-compatible"},
    {0x0, 0x1, "VGA-compatible"},

    /* Mass Storage */
    {0x1, 0x0, "SCSI Bus Controller"},
    {0x1, 0x1, "IDE Controller"},
    {0x1, 0x2, "Floppy Disk Controller"},
    {0x1, 0x3, "IPI Bus Controller"},
    {0x1, 0x4, "RAID Controller"},
    {0x1, 0x5, "ATA Controller"},
    {0x1, 0x6, "Serial ATA Controller"},
    {0x1, 0x7, "Serial Attached SCSI Controller"},
    {0x1, 0x8, "Non-Volatile Memory Controller"},
    {0x1, 0x80, "Other"},

    /* Network Controller */
    {0x2, 0x0, "Ethernet Controller"},
    {0x2, 0x1, "Token Ring Controller"},
    {0x2, 0x2, "FDDI Controller"},
    {0x2, 0x3, "ATM Controller"},
    {0x2, 0x4, "ISDN Controller"},
    {0x2, 0x5, "WorldFip Controller"},
    {0x2, 0x6, "PICMG 2.14 Multi Computing Controller"},
    {0x2, 0x7, "Infiniband Controller"},
    {0x2, 0x8, "Fabric Controller"},
    {0x2, 0x80, "Other"},

    /* Display Controller */
    {0x3, 0x0, "VGA Compatible Controller"},
    {0x3, 0x1, "XGA Controller"},
    {0x3, 0x2, "3D Controller (VGA-incompatible)"},
    {0x3, 0x80, "Other"},

    /* Multimedia Controller */
    {0x4, 0x0, "Multimedia Video Controller"},
    {0x4, 0x1, "Multimedia Audio Controller"},
    {0x4, 0x2, "Computer Telephony Device"},
    {0x4, 0x3, "Audio Device"},
    {0x4, 0x80, "Other"},

    /* Memory Controller */
    {0x5, 0x0, "RAM Controller"},
    {0x5, 0x1, "Flash Controller"},
    {0x5, 0x80, "Other"},

    /* Bridge */
    {0x6, 0x0, "Host Bridge"},
    {0x6, 0x1, "ISA Bridge"},
    {0x6, 0x2, "EISA Bridge"},
    {0x6, 0x3, "MCA Bridge"},
    {0x6, 0x4, "PCI-to-PCI Bridge"},
    {0x6, 0x5, "PCMCIA Bridge"},
    {0x6, 0x6, "NuBus Bridge"},
    {0x6, 0x7, "CardBus Bridge"},
    {0x6, 0x8, "RACEway Bridge"},
    {0x6, 0x9, "PCI-to-PCI Bridge"},
    {0x6, 0x0A, "Infiniband-to-PCI Host Bridge"},
    {0x6, 0x80, "Other"},

    /* Simple Communication Controller */
    {0x7, 0x0, "Serial Controller"},
    {0x7, 0x1, "Parallel Controller"},
    {0x7, 0x2, "Multiport Serial Controller"},
    {0x7, 0x3, "Modem"},
    {0x7, 0x4, "IEEE 488.1/2 (GPIB) Controller"},
    {0x7, 0x5, "Smart Card Controller"},
    {0x7, 0x80, "Other"},

    /* Base System Peripheral */
    {0x8, 0x0, "PIC"},
    {0x8, 0x1, "DMA Controller"},
    {0x8, 0x2, "Timer"},
    {0x8, 0x3, "RTC Controller"},
    {0x8, 0x4, "PCI Hot-Plug Controller"},
    {0x8, 0x5, "SD Host Controller"},
    {0x8, 0x6, "IOMMU"},
    {0x8, 0x80, "Other"},

    /* Input Device Controller */
    {0x9, 0x0, "Keyboard Controller"},
    {0x9, 0x1, "Digitizer Pen"},
    {0x9, 0x2, "Mouse Controller"},
    {0x9, 0x3, "Scanner Controller"},
    {0x9, 0x4, "Gameport Controller"},
    {0x9, 0x80, "Other"},

    /* Docking Station */
    {0xA, 0x0, "Generic"},
    {0xA, 0x80, "Other"},

    /* Processor */
    {0xB, 0x0, "386"},
    {0xB, 0x1, "486"},
    {0xB, 0x2, "Pentium"},
    {0xB, 0x3, "Pentium Pro"},
    {0xB, 0x10, "Alpha"},
    {0xB, 0x20, "PowerPC"},
    {0xB, 0x30, "MIPS"},
    {0xB, 0x40, "Co-processor"},
    {0xB, 0x80, "Other"},

    /* Serial Bus Controller */
    {0xC, 0x0, "FireWire (IEEE 1394) Controller"},
    {0xC, 0x1, "ACCESS Bus Controller"},
    {0xC, 0x2, "SSA"},
    {0xC, 0x3, "USB Controller"},
    {0xC, 0x4, "Fibre Channel"},
    {0xC, 0x5, "SMBus Controller"},
    {0xC, 0x6, "Infiniband Controller"},
    {0xC, 0x7, "IPMI Interface"},
    {0xC, 0x8, "SERCOS Interface (IEC 61491)"},
    {0xC, 0x9, "CANBus Controller"},
    {0xC, 0x80, "Other"},

    /* Wireless Controller */
    {0xD, 0x0, "iRDA Compatible Controller"},
    {0xD, 0x1, "Consumer IR Controller"},
    {0xD, 0x10, "RF Controller"},
    {0xD, 0x11, "Bluetooth Controller"},
    {0xD, 0x12, "Broadband Controller"},
    {0xD, 0x20, "Ethernet Controller (802.1a)"},
    {0xD, 0x21, "Ethernet Controller (802.1b)"},
    {0xD, 0x80, "Other"},

    /* Intelligent Controller */
    {0xE, 0x0, "I20"},

    /* Satellite Communication Controller */
    {0xF, 0x0, "Satellite TV Controller"},
    {0xF, 0x1, "Satellite Audio Controller"},
    {0xF, 0x2, "Satellite Voice Controller"},
    {0xF, 0x3, "Satellite Data Controller"},

    /* Encryption Controller */
    {0x10, 0x0, "Network and Computing Encryption/Decryption"},
    {0x10, 0x10, "Entertainment Encryption/Decryption"},
    {0x10, 0x80, "Other"},

    /* Signal Processing Controller */
    {0x11, 0x0, "DPIO Modules"},
    {0x11, 0x1, "Performance Counters"},
    {0x11, 0x10, "Communication Synchronizer"},
    {0x11, 0x20, "Signal Processing Management"},
    {0x11, 0x80, "Other"},

    /* NULL; this marks the end */
    {0x0, 0x0, NULL}
};

int sub_count = 0;

uint16_t read_pci(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
void write_pci(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg_offset, uint32_t value);
addr_reg get_addr_reg(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

bool device_functions(uint16_t bus, uint16_t device);
uint16_t get_class_id(uint16_t bus, uint16_t device, uint16_t function);
uint16_t get_subclass_id(uint16_t bus, uint16_t device, uint16_t function);

void scan_buses();
void go_through_and_print();
PCIDevice * find_device(uint16_t vendor, uint16_t device);

void init_pci();

PCIDevice * devices[256*32*8];
int device_count = 0;

}
