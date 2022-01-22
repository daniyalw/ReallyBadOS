#pragma once

#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/getch.h>
#include <drivers/keyboard/scanf.h>

struct input_t
{
    void operator>>(char * buff)
    {
        buff = scanf();
    }
};

input_t cin;
