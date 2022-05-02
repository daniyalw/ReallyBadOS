#pragma once

struct Message {
    int from;
    int to;
    int id;
    int pos;

    char *data;
};

Message **messages;
int msg_count = 0;

namespace ipc {

int task_msg_count(int n);
Message *create_msg(int to, int from, char *message);

int send_msg(int to, int from, char *message);
int send_msg(int to, char *message);

int find_messages(int task, Message **msgs);
int find_messages(Message **msgs);

Message *read_last_msg(int task);
Message *read_last_msg();
Message *read_first_msg(int task);
Message *read_first_msg();

void msg_finish(Message * msg, int _task);
void msg_finish(Message * msg);

Message *send_wait(int to, char *message, int timeout);
Message *send_wait(int to, char *message);

}
