#pragma once

typedef struct {
    int from;
    int to;
    int id;
    uint8_t *message;

    int orig_sender;
} Message;

Message **messages;
int message_count = 0;
int id_total = 0;

Message *ipc_create_msg(int from, int to, int orig, uint8_t *message);
void ipc_add_msg_list(Message *msg);
void ipc_move_msgs_down(int pos);
void ipc_read_message_id(int id);
void ipc_read_message(Message *msg);
void ipc_send_message(int from, int to, uint8_t *message);
void ipc_send_message(int to, uint8_t *message);
void ipc_reply_message(Message *msg, uint8_t *message);
Message *ipc_find_messages_pid(Kernel::CPU::pid_t pid);
Message *ipc_send_message_wait(int from, int to, uint8_t *message);
Message *ipc_check_inbox();
