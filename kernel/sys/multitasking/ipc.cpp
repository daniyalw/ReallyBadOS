#include "ipc.h"
#include "task.h"

namespace Kernel {
    namespace IPC {
        Message *create_msg(int from, int to, int orig, int message) {
            Message *msg = new Message();
            // the id part is written outside

            msg->from = from;
            msg->to = to;
            msg->orig_sender = orig;
            msg->message = message;

            return msg;
        }

        void add_msg_list(Message *msg) {
            msg->id = id_total;
            messages[message_count] = msg;
            message_count++;
            id_total++;
        }

        // message at pos will assume to have been read
        void move_msgs_down(int pos) {
            for (int z = pos; z < message_count; z++) {
                messages[z] = messages[z + 1];
            }

            message_count--;
        }

        void read_message_id(int id) {
            free(messages[id]);
            move_msgs_down(id);
        }

        void read_message(Message *msg) {
            read_message_id(msg->id);
        }

        void send_message(int from, int to, int message) {
            Message *msg = create_msg(from, to, from, message);
            add_msg_list(msg);
        }

        void send_message(int to, int message) {
            Message *msg = create_msg(Kernel::CPU::current_task, to, Kernel::CPU::current_task, message);
            add_msg_list(msg);
        }

        void reply_message(Message *msg, int message) {
            Message *_msg = create_msg(msg->to, msg->from, msg->orig_sender, message);
            add_msg_list(_msg);
        }

        Message *find_messages_pid(Kernel::CPU::pid_t pid) {
            for (int z = 0; z < message_count; z++) {
                if (messages[z]->to == pid) {
                    return messages[z];
                }
            }

            return NULL;
        }

        Message *send_message_wait(int from, int to, int message) {
            send_message(from, to, message);
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
    }
}
