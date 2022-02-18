#include <sys/pci/pci.h>

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

void write_pci(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg_offset, uint32_t value)
{
    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (reg_offset & 0xFC);
    outl(0xCF8, id);
    outl(0xCFC, value);
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

                PCIDevice * pci_device = new PCIDevice();

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

                for (int z = 0; z < 6; z++)
                {
                    addr_reg reg = get_addr_reg(bus, device, function, z);
                    if (reg.addr && (reg.type == 1))
                    {
                        pci_device->base = (uint32_t)reg.addr;
                    }
                }

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
        printf("classID: %x ::: SubID: %x ::: DeviceID: 0x%x ::: VendorID: 0x%x\n",
                devices[z]->classID,
                devices[z]->subclassID,
                devices[z]->deviceID,
                devices[z]->vendorID
            );

        if (devices[z]->interrupt != NULL)
            printf("\tInterrupt: %d", devices[z]->interrupt + 32);

        if (devices[z]->interrupt == NULL)
        {
            printf("             ");
            putchar('\t');
        }
        printf("\tinterface: %x\n", devices[z]->interfaceID);
    }

    printf("\n\nTotal devices: %d\n", device_count);
}

PCIDevice * find_device(uint16_t vendor, uint16_t device)
{
    // pci must already be initialized
    for (int z = 0; z < device_count; z++)
        if (devices[z]->vendorID == vendor && devices[z]->deviceID == device)
            return devices[z];

    return NULL;
}

addr_reg get_addr_reg(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    addr_reg result;

    uint32_t barRegister = 0x10 + (bar * sizeof(uint32_t));
    uint32_t bar_v = read_pci(bus, device, function, barRegister);

    if (bar_v == 0)
        return result;

    uint32_t headertype = read_pci(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headertype);

    if (bar >= maxBARs)
        return result;

    write_pci(bus, device, function, barRegister, 0xffffffff);
    uint32_t sizeMask = read_pci(bus, device, function, barRegister);

    write_pci(bus, device, function, barRegister, bar_v);

    result.type = (bar_v & 0x1) ? 1 : 0;

    if(result.type == 0)
    {
        switch((bar_v >> 1) & 0x3)
        {
            case 0: // 32 Bit Mode
                result.addr = (uint32_t)(uintptr_t)(bar_v & ~0xf);
                result.size = ~(sizeMask & ~0xf) + 1;
                result.prefetchable = bar_v & 0x8;
                break;
            case 2: // 64 Bit Mode
                result.size = ~(sizeMask & ~0xf) + 1;
                result.prefetchable = bar_v & 0x8;
                addr_reg sbar = get_addr_reg(bus, device, function, bar + 1);
                result.addr = ((uint32_t)(uintptr_t)(bar_v & ~0xf) + ((sbar.addr & 0xFFFFFFFF) << 32));
                break;
        }
    }
    else
    {
        result.addr = (uint32_t)(bar_v & ~0x3);
        result.size = (uint16_t)(~(sizeMask & ~0x3) + 1);
    }
    return result;
}
