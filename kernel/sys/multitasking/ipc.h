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

int ipc_task_msg_count(int n);
Message *ipc_create_msg(int to, int from, char *message);

int ipc_send_msg(int to, int from, char *message);
int ipc_send_msg(int to, char *message);

int ipc_find_messages(int task, Message **msgs);
int ipc_find_messages(Message **msgs);

Message *ipc_read_last_msg(int task);
Message *ipc_read_last_msg();
Message *ipc_read_first_msg(int task);
Message *ipc_read_first_msg();

void ipc_msg_finish(Message * msg, int _task);
void ipc_msg_finish(Message * msg);

Message *ipc_send_wait(int to, char *message, int timeout);
Message *ipc_send_wait(int to, char *message);
