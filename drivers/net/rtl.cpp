#include <kernel/sys/pci/pci.h>
#include <drivers/net/rtl.h>
#include <sys/io.h>
#include <net/ethernet.h>
#include <bit.h>

namespace Net
{

void rtl8139::power_on() {
    ASSERT(device != NULL);

    Kernel::IO::outb(device->base + RTL_CONFIG1_REG, 0x0);
}

void rtl8139::reset() {
    ASSERT(device != NULL);

    unsigned char ret;

    Kernel::IO::outb(device->base + RTL_CMD, 0x10);

    while (true) {
        ret = Kernel::IO::inb(device->base + RTL_CMD) & 0x10;

        if (!ret)
            break;
    }
}

void rtl8139::init_recv_buffer() {
    ASSERT(device != NULL);

    buffer = (uint32_t)malloc(RTL_BUFSIZE);

    if (!buffer) {
        printf("ERROR: no space for buffer\n");
        return;
    }

    Kernel::IO::outw(device->base + RTL_RBSTART, buffer);
}

void rtl8139::init_irq() {
    ASSERT(device != NULL);

    Kernel::CPU::register_interrupt_handler(device->interrupt + 32, net_irq_handler);
    Kernel::IO::outw(device->base + RTL_IMR, 0x0005);

    log::info("rtl: interrupt %d", device->interrupt + 32);
}

void rtl8139::configure_recv() {
    ASSERT(device != NULL);

    Kernel::IO::outl(device->base + 0x44, 0xF | (1 << 7));
}

void rtl8139::enable_recv() {
    ASSERT(device != NULL);

    Kernel::IO::outb(device->base + RTL_CMD, 0x0C);
}

void rtl8139::send(void *data, int length) {
    log::info("rtl: attemping to send data of length %d across network", length);

    uint8_t *copy = (uint8_t *)malloc(length);
    memcpy(copy, data, length);

    Kernel::IO::outl(device->base + tsad[current_pair], (uint32_t)copy);
    Kernel::IO::outl(device->base + tsd[current_pair], length);

    current_pair++;

    if (current_pair == 3)
        current_pair = 0;

    free(copy);
}

int rtl8139::start() {
    device = Kernel::find_device(RTL_VENDOR, RTL_ID);

    if (device == NULL) {
        // there is no rtl8139
        log::error("rtl: no RTL8139 card detected!\n");
        return 1;
    }

    Kernel::CPU::register_interrupt_handler(47, net_irq_handler);

    uint16_t ret = Kernel::read_pci(device->bus, device->device, device->function, 0x04);

    // we gotta be careful if it is already enabled
    if (!BIT_GET(ret, 2)) {
        BIT_SET(ret, 2);
        Kernel::write_pci(device->bus, device->device, device->function, 0x04, ret);
    }

    power_on();
    reset();
    init_recv_buffer();
    init_irq();
    enable_recv();
    configure_recv();
    find_mac();
    set_name("rtl8139");
    print_mac(mac_addr);

    Net::add_driver(this);

    return 0;
}

void rtl8139::handle_irq(registers_t *regs) {
    ASSERT(device != NULL);

    uint16_t ret = Kernel::IO::inw(device->base + RTL_ISR);

    if (ret & RTL_ISR_RECV_OK) {
        log::info("rtl: packet received");
        Kernel::IO::outw(device->base + RTL_ISR, RTL_ISR_FINISH);

        uint16_t *t = (uint16_t *)(buffer + cptr);
        int length = *t + 1;
        t += 2;

        void *packet = (void *)malloc(length + 1);
        memset((int *)packet, 0, length + 1);
        memcpy(packet, t, length);

        Net::Ethernet::handle_packet((Net::Ethernet::ethernet_frame_t *)packet, length);

        cptr = (cptr + 4 + 3 + length) & (~3);

        if (cptr > RTL_BUFSIZE) {
            cptr -= RTL_BUFSIZE;
        }

        Kernel::IO::outw(device->base + 0x3E, 0x5); // to reset it
    } else if (ret & RTL_ISR_RECV_ERR) {
        log::info("rtl: failed to receive packet");
    } else if (ret & RTL_ISR_SENT_OK) {
        log::info("rtl: sent packet");
    } else if (ret & RTL_ISR_SENT_ERR) {
        log::info("rtl: failed to send packet");
    }

    Kernel::IO::outw(device->base + RTL_ISR, RTL_ISR_FINISH);
}

void rtl8139::find_mac() {
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

void rtl8139::end() {
    free((void *)buffer); // we don't need it anymore
}

}
