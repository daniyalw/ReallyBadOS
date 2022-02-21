#include <net/arp.h>
#include <net/utils.h>
#include <net/ethernet.h>

using namespace Net;

namespace Net
{

namespace ARP
{

void send_request(uint8_t ip_addr[4])
{
    arp_t *arp = (arp_t *)malloc(sizeof(arp_t *));

    arp->hardware_type = ARP_ETHERNET;
    arp->protocol_type = ARP_IP;

    arp->opcode = ARP_REQUEST;

    memset(arp->dst_hardware_addr, 0, 6);
    memcpy(arp->dst_protocol_addr, ip_addr, 4);

    get_src_mac(arp->src_hardware_addr);

    arp->src_protocol_addr[0] = 10;
    arp->src_protocol_addr[1] = 0;
    arp->src_protocol_addr[2] = 2;
    arp->src_protocol_addr[3] = 14;

    send_ethernet_packet(arp->dst_hardware_addr, ETHERNET_ARP, (uint8_t *)arp);
}

}

}
