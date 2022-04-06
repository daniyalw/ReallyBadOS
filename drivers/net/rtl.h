#pragma once

#include <sys/pci/pci.h>
#include <drivers/net/net.h>

namespace Net
{

// wrap in nice #defines
#define RTL_CMD 0x37
#define RTL_CONFIG1_REG 0x52
#define RTL_RBSTART 0x30
#define RTL_IMR 0x3C
#define RTL_VENDOR 0x10EC
#define RTL_ID 0x8139
#define RTL_IDR0 0x0
#define RTL_IDR4 0x4

#define RTL_TSD0 0x10
#define RTL_TSD1 0x14
#define RTL_TSD2 0x18
#define RTL_TSD3 0x1C
#define RTL_TSAD0 0x20
#define RTL_TSAD1 0x24
#define RTL_TSAD2 0x28
#define RTL_TSAD3 0x2C

#define RTL_BUFSIZE 8192 + 16 + 1500

uint8_t tsd[] = {
    RTL_TSD0, RTL_TSD1, RTL_TSD2, RTL_TSD3
};

uint8_t tsad[] = {
    RTL_TSAD0, RTL_TSAD1, RTL_TSAD2, RTL_TSAD3
};

// isr
#define RTL_ISR 0x3E

#define RTL_ISR_RECV_OK 0x1
#define RTL_ISR_RECV_ERR 0x2
#define RTL_ISR_SENT_OK 0x4
#define RTL_ISR_SENT_ERR 0x8

#define RTL_ISR_FINISH 0x5

class rtl8139 : public NetDriver {
private:
    uint32_t buffer;

    int current_pair = 0;

    void power_on();
    void reset();
    void init_recv_buffer();
    void init_irq();
    void configure_recv();
    void enable_recv();
    void find_mac();

    uint32_t cptr = 0;

    void print_mac(uint8_t *_mac) {
        log::info("rtl: successfully enabled network card! MAC: %x:%x:%x:%x:%x:%x", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
    }

public:

    virtual void get_mac(uint8_t *mac) override {
        memcpy(mac, mac_addr, 6);
    }

    virtual void send(void *data, int length) override;
    virtual int start() override;
    virtual void end() override;
    virtual void handle_irq(registers_t *regs) override;
};

}
