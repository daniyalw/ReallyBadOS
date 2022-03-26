#include <net/utils.h>

uint32_t to_big(uint32_t lint) {
    uint32_t b1 = *((uint8_t*)(&lint));
    uint32_t b2 = *((uint8_t*)(&lint) + 1);
    uint32_t b3 = *((uint8_t*)(&lint)  + 2);
    uint32_t b4 = *((uint8_t*)(&lint) + 3);
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
}

uint16_t to_short(uint16_t sint) {
    uint32_t b1 = *((uint8_t*)(&sint));
    uint32_t b2 = *((uint8_t*)(&sint) + 1);
    return (b1 << 8) | (b2);
}

uint8_t *get_src_mac(uint8_t mac_addr[6]) {
    memcpy(mac_addr, rtl8139::mac_addr, 6);

    return (uint8_t *)mac_addr;
}
