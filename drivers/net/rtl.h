#include <sys/pci/pci.h>

namespace Net
{

// wrap in nice #defines
#define RTL_CMD 0x37
#define RTL_CONFIG1_REG 0x52
#define RBSTART 0x30
#define IMR 0x3C
#define ISR 0x3E
#define VENDOR 0x10EC
#define ID 0x8139

struct rtl8139
{
    PCIDevice *device;
    uint32_t buffer;

    void power_on();
    void reset();
    void init_recv_buffer();
    void init_irq();
    void configure_recv();
    void enable_recv();

    void start();
};

}
