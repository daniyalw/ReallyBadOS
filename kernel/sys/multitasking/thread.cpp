#pragma once

#include "task.h"
#include "thread.h"

namespace Kernel {

    namespace CPU {

        pid_t create_thread(char *name, uint32_t begin)
        {
            return Kernel::CPU::create_process(name, begin, true, current_task, 0, NULL);
        }

        pid_t create_thread(char *name, uint32_t begin, int parent)
        {
            return Kernel::CPU::create_process(name, begin, true, parent, 0, NULL);
        }

        pid_t create_thread(char *name, uint32_t begin, int argc, char **argv)
        {
            return Kernel::CPU::create_process(name, begin, true, current_task, argc, argv);
        }

        pid_t create_thread(char *name, uint32_t begin, int parent, int argc, char **argv)
        {
            return Kernel::CPU::create_process(name, begin, true, parent, argc, argv);
        }

    }

}
