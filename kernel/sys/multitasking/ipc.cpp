#include "ipc.h"
#include "task.h"

namespace ipc {

int task_msg_count(int n) {
    if (n < 0 || n >= Kernel::CPU::task_count) {
        return -1;
    }

    int length = 0;

    for (int z = 0; z < msg_count; z++) {
        if (messages[z]->to == n) {
            length++;
        }
    }

    return length;
}

Message * create_msg(int to, int from, char *message) {
    Message * msg = new Message();

    msg->data = message;
    msg->to = to;
    msg->from = from;
    msg->id = task_msg_count(to);
    msg->pos = msg_count;

    return msg;
}

int send_msg(int to, int from, char *message) {
    Message * msg = create_msg(to, from, message);

    if (msg->id == -1) {
        return -1;
    }

    messages[msg_count] = msg;
    msg_count++;

    return 0;
}

int send_msg(int to, char *message) {
    return send_msg(to, Kernel::CPU::current_task, message);
}

Message *send_wait(int to, char *message, int timeout /* useful if a process never replies */) {
    if (send_msg(to, message) == -1) {
        return NULL;
    }

    sleep_sec(timeout);

    return read_last_msg();
}

Message *send_wait(int to, char *message) {
    return send_wait(to, message, 2);
}

int find_messages(int task, Message **msgs) {
    if (task < 0 || task >= Kernel::CPU::task_count) {
        return NULL;
    }

    int c = 0;

    for (int z = 0; z < msg_count; z++) {
        if (messages[z]->to == task) {
            msgs[c] = messages[z];
            c++;
        }
    }

    return c;
}

int find_messages(Message **msgs) {
    return find_messages(Kernel::CPU::current_task, msgs);
}

Message * read_last_msg(int _task) {
    if (_task < 0 || _task >= Kernel::CPU::task_count) {
        return NULL;
    }

    for (int z = msg_count - 1; z >= 0; z--) {
        if (messages[z]->to == _task) {
            return messages[z];
        }
    }

    return NULL;
}

Message * read_last_msg() {
    return read_last_msg(Kernel::CPU::current_task);
}

Message * read_first_msg(int task) {
    if (task < 0 || task >= Kernel::CPU::task_count) {
        return NULL;
    }

    for (int z = 0; z < msg_count; z++) {
        if (messages[z]->to == task) {
            return messages[z];
        }
    }

    return NULL;
}

Message * read_first_msg() {
    return read_first_msg(Kernel::CPU::current_task);
}

void msg_finish(Message * msg, int _task) {
    int id = msg->id; // copy id before destroying the message
    int pos = msg->pos;

    delete msg;

    for (int z = pos; z < msg_count; z++) {
        messages[z] = messages[z + 1];
    }

    msg_count--;
}

void msg_finish(Message *msg) {
    return msg_finish(msg, Kernel::CPU::current_task);
}

}
