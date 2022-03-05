#pragma once

#define MESSAGE_TYPE_REQUEST 1
#define MESSAGE_TYPE_REPLY 2

typedef struct
{
    int message;
    int from, to;
    int type;

    int id;
} message_t;

int send_message(int from, int to, int message, int type);
int send_request(int from, int to, int message);
int send_reply(int from, int to, int message);

message_t *read_message(int task_no, int message_no);

void delete_message(message_t *message);
