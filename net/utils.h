#pragma once

#define ARP_REQUEST 1
#define ARP_REPLY 2
#define ETHERNET_ARP 0x0806
#define ETHERNET_IP  0x0800
#define HARDWARE_ETHERNET 0x01

uint32_t to_big(uint32_t lint);
uint16_t to_short(uint16_t sint);
