#include <net/ethernet.h>
#include <drivers/net/rtl.h>
#include <net/utils.h>
#include <net/arp.h>

using namespace Net;
using namespace Ethernet;

namespace Net
{

namespace Ethernet
{

void send_ethernet_packet(uint8_t dest_mac_addr[6], uint16_t type, int _len, uint8_t *data) {
    ethernet_frame_t *enet = (ethernet_frame_t *)malloc(sizeof(ethernet_frame_t));

    memcpy(enet->dst_mac_addr, dest_mac_addr, 6);
    enet->type = htons(type);
    memcpy(enet + sizeof(ethernet_frame_t), data, _len);
    Net::get_mac(enet->src_mac_addr);

    Net::send(enet, sizeof(ethernet_frame_t));

    free(enet);
}

void handle_packet(ethernet_frame_t *packet, int length) {
    void *data = packet + sizeof(ethernet_frame_t);
    int data_length = length - sizeof(ethernet_frame_t);
    int type = ntohs(packet->type);

    switch (type) {
        case ETHERNET_ARP:
            Net::ARP::handle_arp((Net::ARP::arp_t *)data, data_length);
            break;

        default:
            log::error("net: unknown ethernet packet received of type %d\n", type);
            break;
    }
}

}

}
