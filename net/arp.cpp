#include <net/arp.h>
#include <net/utils.h>
#include <net/ethernet.h>

using namespace Net;

namespace Net
{

namespace ARP
{

void send_request(uint8_t ip_addr[4]) {
    arp_t *arp = (arp_t *)malloc(sizeof(arp_t *));

    arp->hardware_type = htons(ARP_ETHERNET);
    arp->protocol_type = htons(ARP_IP);

    arp->opcode = htons(ARP_REQUEST);

    memset(arp->dst_hardware_addr, 0xFF, 6);
    memcpy(arp->dst_protocol_addr, ip_addr, 4);

    Net::get_mac(arp->src_hardware_addr);
    get_src_ip(arp->src_protocol_addr);

    send_ethernet_packet(arp->dst_hardware_addr, ETHERNET_ARP, sizeof(arp_t), (uint8_t *)arp);
}

void handle_arp(arp_t *packet, int length) {
    // need it for later
    uint8_t dha[6];
    uint8_t dpa[6];

    memcpy(dha, packet->src_hardware_addr, 6);
    memcpy(dpa, packet->src_protocol_addr, 4);

    int type = ntohs(packet->opcode);

    switch (type) {
        case ARP_REQUEST:
            // send mac address back
            Net::get_mac(packet->src_hardware_addr);
            get_src_ip(packet->src_protocol_addr);

            memcpy(packet->dst_hardware_addr, dha, 6);
            memcpy(packet->dst_protocol_addr, dpa, 4);

            packet->opcode = htons(ARP_REPLY);
            packet->hardware_type = htons(HARDWARE_ETHERNET);
            packet->protocol_type = htons(ETHERNET_IP);

            send_ethernet_packet(dha, ETHERNET_ARP, sizeof(arp_t), (uint8_t *)packet);
            break;
        case ARP_REPLY:
            log::warning("net: not equppied to handle ARP_REPLY");

        default:
            log::error("net: unknown ARP operation code received: %d\n", type);
            break;
    }
}

}

}
