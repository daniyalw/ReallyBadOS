#pragma once

#include <drivers/net/rtl.h>

namespace Net
{

namespace Ethernet
{

#define ETHERNET_ARP 0x0806

typedef struct
{
    uint8_t dst_mac_addr[6];
    uint8_t src_mac_addr[6];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_frame_t;

void send_ethernet_packet(uint8_t dest_mac_addr[6], uint16_t type, int _len, uint8_t *data);
void handle_packet(void *packet, int length);

}

}
