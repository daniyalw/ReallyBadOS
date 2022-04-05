#include <net/ethernet.h>
#include <drivers/net/rtl.h>
#include <net/utils.h>

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
    get_src_mac(enet->src_mac_addr);

    rtl8139::send_packet(enet, sizeof(ethernet_frame_t));

    free(enet);
}

void handle_packet(void *packet, int length) {

}

}

}
