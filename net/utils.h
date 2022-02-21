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
