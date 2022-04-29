#pragma once

class NetDriver {
protected:
    uint8_t mac_addr[6] = {0, 0, 0, 0, 0, 0};
    PCI::PCIDevice *device;
    char *name;

    void set_name(char *_name) {
        name = _name;
    }
public:
    virtual void get_mac(uint8_t *mac) {} // buffer (for the MAC to be copied to)
    virtual void send(void *data, int length) {} // data, length
    virtual int start() { return 1; }
    virtual void end() {}
    virtual void handle_irq(registers_t *regs) {}

    char *get_name() {
        return strdup(name);
    }
};


NetDriver **drivers;
int driver_count = 0;
int selected_driver = -1;

void net_irq_handler(registers_t *regs);

namespace Net {

void add_driver(NetDriver *driver);
void close_driver(int driver);
void send(void *data, int length);
void get_mac(uint8_t *mac);

}
