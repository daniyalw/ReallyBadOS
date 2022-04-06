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

uint8_t to_byte(uint8_t byte, int num_bits) {
    uint8_t t = byte << (8 - num_bits);
    return t | (byte >> num_bits);
}

uint8_t htonb(uint8_t byte, int num_bits) {
    return to_byte(byte, num_bits);
}

uint8_t ntohb(uint8_t byte, int num_bits) {
    return to_byte(byte, 8 - num_bits);
}


uint16_t htons(uint16_t hostshort) {
    return to_short(hostshort);
}

uint32_t htonl(uint32_t hostlong) {
    return to_big(hostlong);
}

uint16_t ntohs(uint16_t netshort) {
    return to_short(netshort);
}

uint32_t ntohl(uint32_t netlong) {
    return to_big(netlong);
}

void get_src_ip(uint8_t *ip) {
    ip[0] = 10;
    ip[1] = 0;
    ip[2] = 2;
    ip[3] = 14;
}
