#include "../../stdlib/string.h"
#include "../../stdlib/memory.cpp"
#include "../../kernel/sys/log/log.cpp"

void get_key(unsigned char code);

bool keyboard_lock()
{
    if (!keyboard_locked)
    {
        keyboard_locked = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool keyboard_unlock()
{
    if (keyboard_locked)
    {
        keyboard_locked = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool is_keyboard_locked()
{
    return keyboard_locked;
}

int get_buffer_size()
{
    return buffer_size;
}

void edit_buffer_size(int change)
{
    buffer_size += change;
}

char _getch()
{
    int buffs = buffer_size;
    keyboard_lock();

    while (true)
    {
        if (!is_keyboard_locked())
        {
            char k = buffer[get_buffer_size()-1];
            edit_buffer_size(1);
            return k;
        }
    }
}

// doesn't work properly
char * scanf()
{
    char data[100];
    char k;
    int s = 0;

    while (true)
    {
        k = _getch();

        //putchar(k);

        if (k == '\n')
        {
            char *dd = (char *)malloc(sizeof(char) * s + 1);

            for (int z = 0; z < s; z++)
            {
                dd[z] = data[z];
            }

            return dd;
        }

        data[s] = k;
        s++;
    }
}

static void scan_key(registers_t regs)
{
    // we don't need a safety since this is only run
    // when an interrupt is fired
    current_key = inb(0x60);
    // if we did not finish boot yet this doesn't matter
    if (!booted)
        return;
    // so we already booted
    get_key(current_key);

    return;
}

void get_key(unsigned char code)
{
    char * key;
    unsigned char list_of_codes[] = {0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB9};

    if (code == 0)
    {
        returned = "";
        return;
    }

    if (shifted == 0)
    {
        if (code == 0x02)
            key = "1";
        else if (code == 0x03)
            key = "2";
        else if (code == 0x04)
            key = "3";
        else if (code == 0x05)
            key = "4";
        else if (code == 0x06)
            key = "5";
        else if (code == 0x07)
            key = "6";
        else if (code == 0x08)
            key = "7";
        else if (code == 0x09)
            key = "8";
        else if (code == 0x0A)
            key = "9";
        else if (code == 0x0B)
            key = "0";
        else if (code == 0x0C)
            key = "-";
        else if (code == 0x0D)
            key = "=";
        else if (code == 0x0E)
            key = "\b";
        else if (code == 0x0F)
            key = "    ";
        else if (code == 0x10)
            key = "q";
        else if (code == 0x11)
            key = "w";
        else if (code == 0x12)
            key = "e";
        else if (code == 0x13)
            key = "r";
        else if (code == 0x14)
            key = "t";
        else if (code == 0x15)
            key = "y";
        else if (code == 0x16)
            key = "u";
        else if (code == 0x17)
            key = "i";
        else if (code == 0x18)
            key = "o";
        else if (code == 0x19)
            key = "p";
        else if (code == 0x1A)
            key = "[";
        else if (code == 0x1B)
            key = "]";
        else if (code == 0x1C)
        {
            key = "\n";
        }
        else if (code == 0x3A)
        {
            key = "";
            shifted = 1;
        }
        else if (code == 0x2A)
        {
            shifted = 1;
            key = "";
        }
        else if (code == 0x36)
        {
            shifted = 1;
            key = "";
        }
        else if (code == 0x3A)
        {
            shifted = 1;
            key = "";
        }
        else if (code == 0x1E)
            key = "a";
        else if (code == 0x9F)
            key = "s";
        else if (code == 0x20)
            key = "d";
        else if (code == 0x21)
            key = "f";
        else if (code == 0x22)
            key = "g";
        else if (code == 0x23)
            key = "h";
        else if (code == 0x24)
            key = "j";
        else if (code == 0x25)
            key = "k";
        else if (code == 0x26)
            key = "l";
        else if (code == 0x27)
            key = ";";
        else if (code == 0x28)
            key = "\'";
        else if (code == 0x29)
            key = "\n";
        else if (code == 0x2C)
            key = "z";
        else if (code == 0x2D)
            key = "x";
        else if (code == 0x2E)
            key = "c";
        else if (code == 0x2F)
            key = "v";
        else if (code == 0x30)
            key = "b";
        else if (code == 0x31)
            key = "n";
        else if (code == 0x32)
            key = "m";
        else if (code == 0x33)
            key = ",";
        else if (code == 0x34)
            key = ".";
        else if (code == 0x35)
            key = "/";
        else if (code == 0x39)
            key = " ";
        else
            key = "";
    }
    else
    {
        if (code == 0x02)
            key = "!";
        else if (code == 0x03)
            key = "@";
        else if (code == 0x04)
            key = "#";
        else if (code == 0x05)
            key = "$";
        else if (code == 0x06)
            key = "%";
        else if (code == 0x07)
            key = "^";
        else if (code == 0x08)
            key = "&";
        else if (code == 0x09)
            key = "*";
        else if (code == 0x0A)
            key = "(";
        else if (code == 0x0B)
            key = ")";
        else if (code == 0x0C)
            key = "_";
        else if (code == 0x0D)
            key = "+";
        else if (code == 0x0E)
            key = "\b";
        else if (code == 0x0F)
            key = "    ";
        else if (code == 0x10)
            key = "Q";
        else if (code == 0x11)
            key = "W";
        else if (code == 0x12)
            key = "E";
        else if (code == 0x13)
            key = "R";
        else if (code == 0x14)
            key = "T";
        else if (code == 0x15)
            key = "Y";
        else if (code == 0x16)
            key = "U";
        else if (code == 0x17)
            key = "I";
        else if (code == 0x18)
            key = "O";
        else if (code == 0x19)
            key = "P";
        else if (code == 0x1A)
            key = "{";
        else if (code == 0x1B)
            key = "}";
        else if (code == 0x1C)
            key = "\n";
        else if (code == 0xBA)
        {
            shifted = 0;
            key = "";
        }
        else if (code == 0xAA)
        {
            shifted = 0;
            key = "";
        }
        else if (code == 0xB6)
        {
            shifted = 0;
            key = "";
        }
        else if (code == 0x3A)
        {
            shifted = 0;
            key = "";
        }
        else if (code == 0x1E)
            key = "A";
        else if (code == 0x1F)
            key = "S";
        else if (code == 0x20)
            key = "D";
        else if (code == 0x21)
            key = "F";
        else if (code == 0x22)
            key = "G";
        else if (code == 0x23)
            key = "H";
        else if (code == 0x24)
            key = "J";
        else if (code == 0x25)
            key = "K";
        else if (code == 0x26)
            key = "L";
        else if (code == 0x27)
            key = ":";
        else if (code == 0x28)
            key = "\"";
        else if (code == 0x28)
            key = "\n";
        else if (code == 0x2C)
            key = "Z";
        else if (code == 0x2D)
            key = "X";
        else if (code == 0x2E)
            key = "C";
        else if (code == 0x2F)
            key = "V";
        else if (code == 0x30)
            key = "B";
        else if (code == 0x31)
            key = "N";
        else if (code == 0x32)
            key = "M";
        else if (code == 0x33)
            key = "<";
        else if (code == 0x34)
            key = ">";
        else if (code == 0x35)
            key = "?";
        else if (code == 0x39)
            key = " ";
        else
            key = "";
    }
    if (key != "") {
        buffer[buffer_size] = key[0];
        buffer_size++;
        keyboard_unlock();
    }
    /*
    if (key != "") {
        if (key == "\n")
        {
            printf("\n\n");
            rc(buff);
            clears(buff);
            bf = 0;
            printf("\n\n/> ");
        } else {
            if (key != "\b")
            {
                printf(key);
                for (int z = 0; z < len(key); z++)
                {
                    buff[z+bf] = key[z];
                    bf++;
                }
            }
            else
            {
                if (text_x > 3)
                {
                    buff[bf] = ' ';
                    bf--;
                    text_x--;
                    putchar(' ');
                    text_x--;
                    set_hardware_cursor(text_y, text_x);
                }
            }

        }
    }
    */
}


void init_keyboard() {
    system_log("Enabled keyboard.\n");
    // register the interrupt
   register_interrupt_handler(IRQ1, scan_key);
}