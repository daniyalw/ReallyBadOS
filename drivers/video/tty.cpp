#include "tty.h"

void TTY::putc(char c) {
    if (_state != TTY_CLEAN) {
        return;
    }

    if (_offset == bufsz) {
        _state = TTY_EOM;
        return;
    }

    _buffer[_offset] = c;
    _offset++;
}

int TTY::puts(char *str) {
    int done = 0;

    for (int z = 0; z < strlen(str); z++) {
        this->putc(str[z]);
        done++;
    }

    return done;
}

int TTY::printf(char *fmt, ...) {
    va_list va;
    int length = strlen(fmt);

    va_start(va, fmt);
    char out[length + 1];
    memset(out, 0, length + 1);
    vsprintf(out, fmt, va);
    va_end(va);

    return this->puts(out);
}

int TTY::read(char *buf, int _offset_, int sz) {
    if (_offset <= _offset_) {
        return 1;
    }

    for (int z = 0; z < sz; z++) {
        if ((_offset_ + z) == _offset) {
            break;
        }

        buf[z] = _buffer[_offset_ + z];
    }

    buf[sz + 1] = 0;

    return 0;
}
