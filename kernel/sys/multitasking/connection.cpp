#include "connection.h"

void send_data(int to, char *data) {
    Data *d = new Data();
    d->to = to;
    d->from = Kernel::CPU::current_task;
    strncpy(d->data, data, 100);
    ipc_send_msg(to, (char *)d);
}

Data **get_all_data() {
    Data **data;
    Message **msgs;

    for (int z = 0; z < ipc_find_messages(msgs); z++) {
        data[z] = (Data *)msgs[z]->data;
    }

    return data;
}

Data *get_last() {
    return (Data *)ipc_read_last_msg()->data;
}

int sid_to_pid(int sid) {
    for (int z = 0; z < server_count; z++) {
        if (servers[z]->id() == sid) {
            return servers[z]->thread();
        }
    }

    return -1;
}

int find_server(int serv) {
    for (int z = 0; z < server_count; z++) {
        if (servers[z]->id() == serv) {
            return z;
        }
    }

    return -1;
}
