#include <kernel/sys/pci/pci.h>
#include <drivers/net/rtl.h>
#include <sys/io.h>

using namespace Net;
using namespace rtl8139;

namespace Net
{

namespace rtl8139
{

void power_on() {
    ASSERT(device != NULL);

    Kernel::IO::outb(device->base + RTL_CONFIG1_REG, 0x0);
}

void reset() {
    ASSERT(device != NULL);

    unsigned char ret;

    Kernel::IO::outb(device->base + RTL_CMD, 0x10);

    while (true) {
        ret = Kernel::IO::inb(device->base + RTL_CMD) & 0x10;

        if (!ret)
            break;
    }
}

void init_recv_buffer() {
    ASSERT(device != NULL);

    buffer = (uint32_t)malloc(8192 + 16 + 1500);
    Kernel::IO::outw(device->base + RTL_RBSTART, buffer);
}

void init_irq() {
    ASSERT(device != NULL);

    Kernel::CPU::register_interrupt_handler(device->interrupt + 32, handle_irq);
    Kernel::IO::outw(device->base + RTL_IMR, 0x0005);

    log::info("rtl: interrupt %d", device->interrupt + 32);
}

void configure_recv() {
    ASSERT(device != NULL);

    Kernel::IO::outl(device->base + 0x44, 0xf | (1 << 7));
}

void enable_recv() {
    ASSERT(device != NULL);

    Kernel::IO::outb(device->base + RTL_CMD, 0x0C);
}

void handle_irq(registers_t *regs) {
    ASSERT(device != NULL);

    uint16_t ret = Kernel::IO::inw(device->base + RTL_ISR);

    if (ret & RTL_ISR_RECV_OK) {
        log::info("rtl: packet received");
    } else if (ret & RTL_ISR_RECV_ERR) {
        log::info("rtl: failed to receive packet");
    } else if (ret & RTL_ISR_SENT_OK) {
        log::info("rtl: sent packet");
    } else if (ret & RTL_ISR_SENT_ERR) {
        log::info("rtl: failed to send packet");
    }

    Kernel::IO::outw(device->base + RTL_ISR, RTL_ISR_FINISH);
}

void send_packet(void *data, int length) {
    log::info("rtl: attemping to send data of length %d across network", length);

    uint8_t *copy = (uint8_t *)malloc(length);
    memcpy(copy, data, length);

    Kernel::IO::outl(device->base + tsad[current_pair], (uint32_t)&copy);
    Kernel::IO::outl(device->base + tsd[current_pair], length);

    current_pair++;

    if (current_pair == 3)
        current_pair = 0;

    free(copy);
}

void start() {
    device = Kernel::find_device(RTL_VENDOR, RTL_ID);

    if (device == NULL) {
        // there is no rtl8139
        log::error("rtl: no RTL8139 card detected!\n");
        return;
    }

    Kernel::CPU::register_interrupt_handler(47, handle_irq);

    power_on();
    reset();
    init_recv_buffer();
    init_irq();
    configure_recv();
    enable_recv();
    find_mac();

    log::info("rtl: successfully enabled network card! MAC: %x:%x:%x:%x:%x:%x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void find_mac() {
    ASSERT(device != NULL);

    uint32_t m1 = Kernel::IO::inl(device->base + RTL_IDR0);
    uint16_t m2 = Kernel::IO::inw(device->base + RTL_IDR4);

    mac_addr[0] = m1 >> 0;
    mac_addr[1] = m1 >> 8;
    mac_addr[2] = m1 >> 16;
    mac_addr[3] = m1 >> 24;
    mac_addr[4] = m2 >> 0;
    mac_addr[5] = m2 >> 8;
}

}

}
