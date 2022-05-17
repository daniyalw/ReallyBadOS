#pragma once

#include <sys/descriptors/isr.h>
#include <memory.h>

#define KEYBOARD_CODE 0x60

unsigned char current_key;

bool terminal_on = false;

bool keyboard_locked;
bool scanf_on = false;

char buffer[1000];
int total_size_b = 128;
int buffer_size = 1;
int shifted = 0;

void get_key(unsigned char code);
bool keyboard_lock();
bool keyboard_unlock();
bool is_keyboard_locked();
int get_buffer_size();
void edit_buffer_size(int change);
static void scan_key(registers_t regs);
