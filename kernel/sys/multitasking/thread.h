#pragma once

namespace Kernel {

    namespace CPU {

        pid_t create_thread(char *name, uint32_t begin, int parent);
        pid_t create_thread(char *name, uint32_t begin);
        pid_t create_thread(char *name, uint32_t begin, int argc, char **argv);
        pid_t create_thread(char *name, uint32_t begin, int parent, int argc, char **argv);

    }

}
