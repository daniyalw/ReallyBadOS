#include "ipc.h"
#include "task.h"

Message *ipc_create_msg(int from, int to, int orig, uint8_t *message) {
    Message *msg = new Message();
    // the id part is written outside

    msg->from = from;
    msg->to = to;
    msg->orig_sender = orig;
    msg->message = message;

    return msg;
}

void ipc_add_msg_list(Message *msg) {
    msg->id = id_total;
    messages[message_count] = msg;
    message_count++;
    id_total++;
}

        // message at pos will assume to have been read
void ipc_move_msgs_down(int pos) {
    for (int z = pos; z < message_count; z++) {
        messages[z] = messages[z + 1];
    }

    message_count--;
}

void ipc_read_message_id(int id) {
    free(messages[id]);
    ipc_move_msgs_down(id);
}

void ipc_read_message(Message *msg) {
    ipc_read_message_id(msg->id);
}

void ipc_send_message(int from, int to, uint8_t *message) {
    Message *msg = ipc_create_msg(from, to, from, message);
    ipc_add_msg_list(msg);
}

void ipc_send_message(int to, uint8_t *message) {
    Message *msg = ipc_create_msg(Kernel::CPU::current_task, to, Kernel::CPU::current_task, message);
    ipc_add_msg_list(msg);
}

void ipc_reply_message(Message *msg, uint8_t *message) {
    Message *_msg = ipc_create_msg(msg->to, msg->from, msg->orig_sender, message);
    ipc_add_msg_list(_msg);
}

Message *ipc_find_messages_pid(Kernel::CPU::pid_t pid) {
    for (int z = 0; z < message_count; z++) {
        if (messages[z]->to == pid) {
            return messages[z];
        }
    }

    return NULL;
}

Message *ipc_send_message_wait(int from, int to, uint8_t *message) {
    ipc_send_message(from, to, message);
    int id = messages[message_count - 1]->id;

    while (true) {
        for (int z = 0; z < message_count; z++) {
            if (messages[z]->from == to && messages[z]->to == from && messages[z]->orig_sender == from) {
                return messages[z];
            }
        }
    }

    return NULL;
}

Message *ipc_check_inbox() {
    return ipc_find_messages_pid(current_task);
}
