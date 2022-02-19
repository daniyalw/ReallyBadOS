#include <net/arp.h>

namespace Net
{

namespace ARP
{

void send_arp_packet(uint8_t *dst_hd, uint8_t *dst_p)
{
    arp_t *arp = (arp_t *)malloc(sizeof(arp_t *));

    // TODO: yes, what a great idea to hardcode the IP
    arp->src_protocol[0] = 10;
    arp->src_protocol[1] = 0;
    arp->src_protocol[2] = 2;
    arp->src_protocol[3] = 14;

    memcpy(arp->dst_hardware_type, dst_hd, 6);
    memcpy(arp->dst_protocol, dst_p, 4);

    arp->opcode = to_short(ARP_REQUEST);

    arp->opcode = to_short(ARP_REQUEST);

    arp->hardware_addr_len = 6;
    arp->protocol_addr_len = 4;

    arp->hardware_type = to_short(HARDWARE_ETHERNET);

    arp->protocol = to_short(ETHERNET_IP);

    Net::Ethernet::send_ethernet_packet(arp->dst_hardware_type, ETHERNET_ARP, (uint8_t *)&arp);

    free(arp);
}

}

}
