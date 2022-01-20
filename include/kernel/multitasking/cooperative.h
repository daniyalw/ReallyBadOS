namespace Cooperative {

typedef struct
{
    char *name;
    uint32_t addr;
    bool null = false;
} task_t;

task_t tasks[10];
int task_count = 0;

task_t current;
int current_id = 0;

bool began = false;

void create_task(char * name, uint32_t addr);
void switch_task();
void yield();
void update_addr(uint32_t addr);
void exit();
void task_ls();

}
