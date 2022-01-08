#pragma once

#include <kernel/isr.h>

int shifted = 0;
char * returned;
unsigned char current_key;

bool terminal_on = false;

bool keyboard_locked;
bool scanf_on = false;

char buffer[1000];
int buffer_size = 1;
int entered_len = 0;

void get_key(unsigned char code);
bool keyboard_lock();
bool keyboard_unlock();
bool is_keyboard_locked();
int get_buffer_size();
void edit_buffer_size(int change);
char _getch();
char * scanf(char * dd);
static void scan_key(registers_t regs);
void get_key(unsigned char code);

struct input_t
{
    void operator>>(char * buff)
    {
        scanf(buff);
    }
};

input_t cin;
