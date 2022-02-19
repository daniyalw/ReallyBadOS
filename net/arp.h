#pragma once

namespace Net
{

namespace ARP
{

typedef struct
{
    uint16_t hardware_type;
    uint16_t protocol;

    uint8_t hardware_addr_len;
    uint8_t protocol_addr_len;

    uint16_t opcode;

    uint8_t *src_hardware_type;
    uint8_t *src_protocol;

    uint8_t *dst_hardware_type;
    uint8_t *dst_protocol;
} arp_t;

void send_arp_packet(uint8_t *dst_hd, uint8_t *dst_p);

}

}
