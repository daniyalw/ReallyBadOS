#pragma once

#include <sys/pci/pci.h>

namespace Net
{

namespace rtl8139
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

PCIDevice *device;
uint32_t buffer;

int current_pair = 0;

uint8_t mac_addr[6]; // 6-byte

void power_on();
void reset();

void init_recv_buffer();
void configure_recv();
void enable_recv();

void find_mac();

void send_packet(void *data, int length);

void init_irq();
void handle_irq(registers_t regs);

void start();

}

}
