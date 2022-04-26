#pragma once

int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
bool booted = false;
int back_buffer[1024*768]; // back buffer for gui
//unsigned int initial_stack;
// this is the /> _
char current_display[128];
int current_display_len = 0;
bool in_ring3 = false;

char *user;
int current_login = 1;

void login();
