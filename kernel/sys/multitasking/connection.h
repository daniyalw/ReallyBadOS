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
    int _id = -1;
    int _thread = -1;

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

    int id() {
        return _id;
    }

    int thread() {
        return _thread;
    }

    void close() {
        for (int z = client_count - 1; z >= 0; z--) {
            server_disconnect_client(clients[z]);
        }

        Kernel::CPU::exit(0);
    }

    int init(char *name, int sid, auto mainloop) {
        for (int z = 0; z < server_count; z++) {
            if (servers[z]->id() == sid) {
                return -1;
            }
        }

        _id = sid;
        _thread = Kernel::CPU::create_process(name, (uint32_t)mainloop);
        servers[server_count] = this;
        server_count++;

        return _id;
    }
};
