#pragma once

#include <kernel/isr.h>
#include <memory.h>

int shifted = 0;
char * returned;
unsigned char current_key;

bool terminal_on = false;

bool keyboard_locked;
bool scanf_on = false;

char * buffer = (char *)malloc(128);
int total_size_b = 128;
int buffer_size = 1;
int entered_len = 0;

void get_key(unsigned char code);
bool keyboard_lock();
bool keyboard_unlock();
bool is_keyboard_locked();
int get_buffer_size();
void edit_buffer_size(int change);
char _getch();
char * scanf();
static void scan_key(registers_t regs);
void get_key(unsigned char code);

struct input_t
{
    void operator>>(char * buff)
    {
        buff = scanf();
    }
};

input_t cin;
