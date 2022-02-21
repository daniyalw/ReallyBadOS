#pragma once

namespace Net
{

namespace ARP
{

typedef struct
{
    uint16_t hardware_type;
    uint16_t protocol_type;

    uint8_t hardware_addr_len = 6;
    uint8_t protocol_addr_len = 4;

    uint16_t opcode;

    uint8_t src_hardware_addr[6];
    uint8_t src_protocol_addr[4];

    uint8_t dst_hardware_addr[6];
    uint8_t dst_protocol_addr[4];
} arp_t;

typedef struct
{
    uint8_t mac_addr[6];
    uint8_t ip_addr[4];
} arp_table_t;

arp_table_t arp_table[256];

void send_request(uint8_t ip_addr[4]);

}

}
