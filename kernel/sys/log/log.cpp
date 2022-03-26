#pragma once
#include <sys/log/log.h>
#include <sys/serial.h>
#include <string.h>

namespace log {
    void info(char *data, ...) {
        va_list va;
        auto light_blue = get_color("40", "36");

        va_start(va, data);
        p_template(light_blue, "Info: ", data, va);
        va_end(va);

        Kernel::output_serial_char('\n');
    }

    void error(char *data, ...) {
        va_list va;
        auto red = get_color("40", "1;31");

        va_start(va, data);
        p_template(red, "Error: ", data, va);
        va_end(va);

        Kernel::output_serial_char('\n');
    }

    void warning(char *data, ...) {
        va_list va;
        auto yellow = get_color("40", "1;33");

        va_start(va, data);
        p_template(yellow, "Warning: ", data, va);
        va_end(va);

        Kernel::output_serial_char('\n');
    }

    void warn(char *data, ...) {
        va_list va;
        auto yellow = get_color("40", "1;33");

        va_start(va, data);
        p_template(yellow, "Warning: ", data, va);
        va_end(va);

        Kernel::output_serial_char('\n');
    }
}

namespace Kernel {

void init_logging() {}

}
