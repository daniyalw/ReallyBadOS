#pragma once

int keycode_length = 50;
int shift_code_length = 6;
int ctrl_codes_length = 2;

unsigned char keycodes[] = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
    0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x1A, 0x1B, 0x1C, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x27, 0x28, 0x29, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x39
};

unsigned char shifted_codes[] = {
    0x3A, 0x2A, 0x36, 0xBA, 0xAA, 0xB6
};

// same codes sent by right and left ctrl
unsigned char ctrl_codes[] = {
    0x1D, 0x9D
};

unsigned char key_up = 0x48;
unsigned char key_down = 0x50;

char letters_lower[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', ' ',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\n', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', '/', ' '
};

char letters_upper[] = {
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', ' ',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '\n', 'Z', 'X', 'C',
    'V', 'B', 'N', 'M', '<', '>', '?', ' '
};
