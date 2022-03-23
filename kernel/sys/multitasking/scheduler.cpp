#include "scheduler.h"
#include "task.h"

namespace Kernel {

    namespace CPU {

        pid_t schedule()
        {
            // simple round-robin scheduler
            Kernel::CPU::current_task++;

            if (Kernel::CPU::current_task >= Kernel::CPU::task_count)
                Kernel::CPU::current_task = 0;

            while (Kernel::CPU::tasks[Kernel::CPU::current_task]->null || Kernel::CPU::tasks[Kernel::CPU::current_task]->blocked)
            {
                Kernel::CPU::current_task++;

                if (Kernel::CPU::current_task >= Kernel::CPU::task_count)
                    Kernel::CPU::current_task = 0;
            }

            return Kernel::CPU::current_task;
        }

    }

}
