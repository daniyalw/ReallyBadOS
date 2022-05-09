#include "scheduler.h"
#include "task.h"

using namespace Kernel;
using namespace CPU;

namespace Kernel {

    namespace CPU {

        pid_t schedule() {
            // simple round-robin scheduler
            current_task++;

            if (current_task >= task_count)
                current_task = 0;

            while (tasks[current_task]->null || tasks[current_task]->blocked)
            {
                current_task++;

                if (current_task >= task_count)
                    current_task = 0;
            }

            return current_task;
        }

    }

}
