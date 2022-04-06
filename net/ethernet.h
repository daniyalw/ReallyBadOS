#pragma once

#include <drivers/net/rtl.h>

namespace Net
{

namespace Ethernet
{

typedef struct
{
    uint8_t dst_mac_addr[6];
    uint8_t src_mac_addr[6];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_frame_t;

void send_ethernet_packet(uint8_t dest_mac_addr[6], uint16_t type, int _len, uint8_t *data);
void handle_packet(ethernet_frame_t *packet, int length);

}

}
