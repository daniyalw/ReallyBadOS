#include "node.h"
#include "configs.h"

int init_configs_fs() {
    return make_dir("configs", "/");
}
