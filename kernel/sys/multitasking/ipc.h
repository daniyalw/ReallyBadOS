#pragma once

namespace Kernel {
    namespace IPC {
        typedef struct {
            int from;
            int to;
            int id;
            int message;

            int orig_sender;
        } Message;

        Message **messages;
        int message_count = 0;
        int id_total = 0;

        Message *create_msg(int from, int to, int orig, int message);
        void add_msg_list(Message *msg);
        void move_msgs_down(int pos);
        void read_message_id(int id);
        void read_message(Message *msg);
        void send_message(int from, int to, int message);
        void send_message(int to, int message);
        void reply_message(Message *msg, int message);
        Message *find_messages_pid(Kernel::CPU::pid_t pid);
        Message *send_message_wait(int from, int to, int message);
    }
}
