#include "net.h"

void net_irq_handler(registers_t *regs) {
   if (selected_driver >= 0) {
       char *name = drivers[selected_driver]->get_name();
       DEBUG("Driver '%s' handling IRQ\n", name);
       free(name);
       drivers[selected_driver]->handle_irq(regs);
   } else {
       DEBUG("No driver installed!\n");
   }
}

namespace Net {

void add_driver(NetDriver *driver) {
    drivers[driver_count] = driver;
    driver_count++;

    if (driver_count == 1) {
        selected_driver = 0;
    }
}

void close_driver(int driver) {
    if (driver < 0 || driver >= driver_count) {
        return;
    }

    if (driver == selected_driver) {
        selected_driver = -1;
    }

    drivers[driver]->end();
    free(drivers[driver]);

    for (int z = driver; z < driver_count; z++) {
        drivers[z] = drivers[z + 1];
    }

    driver_count--;

    // if we close the current driver we want to be able to fallback to another one
    if (selected_driver == -1) {
        if (driver_count > 0) {
            selected_driver = 0;
        }
    }
}

void send(void *data, int length) {
    if (selected_driver >= 0) {
        drivers[selected_driver]->send(data, length);
    }
}

void get_mac(uint8_t *mac) {
    if (selected_driver >= 0) {
        drivers[selected_driver]->get_mac(mac);
    }
}

}
