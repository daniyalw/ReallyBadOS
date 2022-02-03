#include <kernel/sys/pci/pci.h> // we need pci configurations
#include <drivers/net/rtl.h>
#include <sys/io.h>

namespace Net
{

void rtl8139::power_on()
{
    outb(device->base + RTL_CONFIG1_REG, 0x0);
}

void rtl8139::reset()
{
    unsigned char ret;

    outb(device->base + RTL_CMD, 0x10);

    while (true)
    {
        ret = inb(device->base + RTL_CMD) & 0x10;

        if (!ret)
            break;
    }
}

void rtl8139::init_recv_buffer()
{
    buffer = (uint32_t)malloc(8192 + 16 + 1500);
    outw(device->base + RBSTART, buffer);
}

void rtl8139::init_irq()
{
    outw(device->base + IMR, 0x0005);
}

void rtl8139::configure_recv()
{
    outl(device->base + 0x44, 0xf | (1 << 7));
}

void rtl8139::enable_recv()
{
    outb(device->base + RTL_CMD, 0x0C);
}

void rtl8139::start()
{
    device = find_device(VENDOR, ID);

    if (device == NULL)
    {
        // there is no rtl8139
        error("no RTL8139 card detected!\n");
        return;
    }

    this->power_on();
    this->reset();
    this->init_recv_buffer();
    this->init_irq();
    this->configure_recv();
    this->enable_recv();

    info("successfully enabled network card!\n");
}

}
