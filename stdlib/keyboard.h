#pragma once

#include <keyboard/keyboard.h>
#include <keyboard/getch.h>
#include <keyboard/scanf.h>

struct input_t
{
    void operator>>(char * buff) {
        buff = scanf();
    }
};

input_t cin;
