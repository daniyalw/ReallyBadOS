#include "message.h"
#include "task.h"

void drop_message(message_t *message, task_t *task)
{
    task->messages[task->message_count] = message;
    task->message_count++;
}

int send_message(int from, int to, int msg, int type)
{
    message_t *message = (message_t *)malloc(sizeof(message_t *));

    message->from = from;
    message->to = to;
    message->message = msg;
    message->type = type;

    if (from < 0 || from >= task_count)
    {
        free(message);
        return 1;
    }

    if (to < 0 || to >= task_count)
    {
        free(message);
        return 1;
    }

    if (from == to)
    {
        free(message);
        return 1;
    }

    task_t *from_task = (task_t *)&tasks[from];
    task_t *to_task = (task_t *)&tasks[to];

    message->id = to_task->message_count;

    if (from_task->null || to_task->null)
    {
        free(message);
        return 1;
    }

    drop_message(message, to_task);

    return 0;
}

message_t *read_message(int task_no, int message_no)
{
    if (task_no < 0 || task_no >= task_count)
        return NULL;

    task_t *task = (task_t *)&tasks[task_no];

    if (message_no < 0 || message_no >= task->message_count)
        return NULL;

    return task->messages[message_no];
}

int send_request(int from, int to, int message)
{
    return send_message(from, to, message, MESSAGE_TYPE_REQUEST);
}

int send_reply(int from, int to, int message)
{
    return send_message(from, to, message, MESSAGE_TYPE_REPLY);
}

void delete_message(message_t *message)
{
    task_t *task = (task_t *)&tasks[message->to];

    free(task->messages[message->id]);

    for (int z = message->id; z < task->message_count - 1; z++)
    {
        task->messages[z] = task->messages[z + 1];
    }

    task->message_count--;
}
