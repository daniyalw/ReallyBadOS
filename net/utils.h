#pragma once

#define ETHERNET_ARP 0x0806
#define ETHERNET_IP  0x0800

#define HARDWARE_ETHERNET 0x01

#define ARP_REQUEST 0x1
#define ARP_REPLY 0x2
#define ARP_ETHERNET 0x1
#define ARP_IP 0x0800

uint32_t to_big(uint32_t lint);
uint16_t to_short(uint16_t sint);
uint8_t *get_src_mac(uint8_t mac_addr[6]);

uint8_t to_byte(uint8_t byte, int num_bits);
uint8_t htonb(uint8_t byte, int num_bits);
uint8_t ntohb(uint8_t byte, int num_bits);
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
uint16_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);
