#pragma once

enum TTYStates {
    TTY_CLEAN,
    TTY_ERROR,
    TTY_EOM
};

typedef int (*push_t)(char*);

class TTY {
    char *_buffer;
    int bufsz = 0;
    int _offset = 0;
    int _state = TTY_CLEAN;

public:
    TTY(char *buf, int max) {
        _buffer = buf;
        bufsz = max;
    }

    TTY() {
        bufsz = 2001; // enough for 80 * 25
        _buffer = (char *)malloc(bufsz);
        memset(_buffer, 0, bufsz);
    }

    char *buffer() {
        return _buffer;
    }

    int offset() {
        return _offset;
    }

    int bufsize() {
        return bufsz;
    }

    int state() {
        return _state;
    }

    void putc(char c);
    int puts(char *str);
    int printf(char *fmt, ...);
    int read(char *buf, int _offset_, int sz);
    push_t _push;

    void set_push(push_t push) {
        _push = push;
    }

    int push() {
        return _push(_buffer);
    }
};
