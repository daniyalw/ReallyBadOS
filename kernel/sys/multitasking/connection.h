#pragma once

#include "task.h"

struct Data {
    int to;
    int from;
    char data[98];
};

void send_data(int to, char *data);
Data **get_all_data();
Data *get_last();
int sid_to_pid(int sid);

class Server;
Server **servers;
int server_count = 0;

class Server {
    int clients[10];
    int client_count = 0;
    int id = -1;
    int thread = -1;

public:
    int server_find_client(int id) {
        for (int z = 0; z < client_count; z++) {
            if (clients[z] == id) {
                return z;
            }
        }

        return -1;
    }

    int server_disconnect_client(int id) {
        int pos;

        if ((pos = server_find_client(id)) >= 0) {
            for (int z = pos; z < client_count; z++) {
                clients[z] = clients[z + 1];
            }

            client_count--;

            return 0;
        } else {
            return 1;
        }
    }

    int server_connect_client(int client) {
        if (server_find_client(client) < 0) {
            clients[client_count] = client;
            client_count++;

            return 0;
        }

        return 1;
    }

    int getid() {
        return id;
    }

    int getthread() {
        return thread;
    }

    int init(char *name, int sid, auto mainloop) {
        for (int z = 0; z < server_count; z++) {
            if (servers[z]->getid() == sid) {
                return -1;
            }
        }

        id = sid;
        thread = Kernel::CPU::create_process(name, (uint32_t)mainloop);
        servers[server_count] = this;
        server_count++;

        return id;
    }
};
