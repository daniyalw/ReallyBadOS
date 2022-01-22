#include <kernel/pci.h>

uint16_t read_pci(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    uint32_t address;
    uint16_t tmp = 0;

    address = (uint32_t)(((uint32_t)bus << 16) | ((uint32_t)slot << 11) |
              ((uint32_t)function << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    outl(0xCF8, address);
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

bool device_functions(uint16_t bus, uint16_t device)
{
    return read_pci(bus, device, 0, 0x0E) & (1 << 7);
}

uint16_t get_class_id(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t res = read_pci(bus, device, function, 0xA);
        return res >> 8;
}

uint16_t get_subclass_id(uint16_t bus, uint16_t device, uint16_t function)
{
        uint32_t res = read_pci(bus, device, function, 0x08);
        return (res & 0xFF);
}

void scan_buses()
{
    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            bool result = device_functions(bus, device);
            int function_count;

            if (result)
                function_count = 8;
            else
                function_count = 1;

            for (int function = 0; function < function_count; function++)
            {
                uint16_t res = read_pci(bus, device, function, 0x0);

                if (res == 0xFFFF)
                    continue;

                PCIDevice * pci_device = (PCIDevice *)malloc(sizeof(pci_device));

                pci_device->bus = bus;
                pci_device->device = device;
                pci_device->function = function;
                pci_device->deviceID = read_pci(bus, device, function, 0x02); // 0x2 is from osdev->org
                pci_device->vendorID = res;
                pci_device->classID = get_class_id(bus, device, function);
                pci_device->interrupt = read_pci(bus, device, function, 0x3C);
                pci_device->subclassID = get_subclass_id(bus, device, function);
                pci_device->revisionID = read_pci(bus, device, function, 0x08);
                pci_device->interfaceID = read_pci(bus, device, function, 0x09);

                devices[device_count] = pci_device;
                device_count++;
            }
        }
    }
}

void go_through_and_print()
{
    for (int z = 0; z < device_count; z++)
    {
        printf("classID: %x   :::   SubID: %x\n", devices[z]->classID, devices[z]->subclassID);
    }

    printf("\n\nTotal devices: %d\n", device_count);
}

PCIDevice * find_device(uint16_t vendor, uint16_t device)
{
    // pic must already be initialized
    for (int z = 0; z < device_count; z++)
        if (devices[z]->vendorID == vendor && devices[z]->deviceID == device)
            return devices[z];

    return NULL;
}
