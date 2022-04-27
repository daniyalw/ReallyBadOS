#include "port.h"

int port_read(fs_node_t *node, int offset, int size, char *buffer) {
    switch (size) {
        case 1:
            buffer[0] = Kernel::IO::inb(offset);
            break;
        case 2:
            ((uint16_t *)buffer)[0] = Kernel::IO::inb(offset);
            break;
        case 4:
            ((uint32_t *)buffer)[0] = Kernel::IO::inl(offset);
            break;

        default:
            for (unsigned int i = 0; i < size; ++i) {
                buffer[i] = Kernel::IO::inb(offset + i);
            }

            break;
    }

    return size;
}

int port_write(fs_node_t *node, int offset, int size, char *buffer) {
    switch (size) {
        case 1:
            Kernel::IO::outb(offset, buffer[0]);
            break;
        case 2:
            Kernel::IO::outb(offset, ((uint16_t*)buffer)[0]);
            break;
        case 4:
            Kernel::IO::outl(offset, ((uint32_t*)buffer)[0]);
            break;
        default:
            for (unsigned int i = 0; i < size; ++i) {
                Kernel::IO::outb(offset +i, buffer[i]);
            }
            break;
    }

    return size;
}

int init_portfs() {
    return create_file("port", "/dev", port_read, port_write);
}
