void get_key(unsigned char code);

static void scanf(registers_t regs)
{
    current_key = inb(0x60);
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
        if (code == 0x82)
            key = "1";
        else if (code == 0x83)
            key = "2";
        else if (code == 0x84)
            key = "3";
        else if (code == 0x85)
            key = "4";
        else if (code == 0x86)
            key = "5";
        else if (code == 0x87)
            key = "6";
        else if (code == 0x88)
            key = "7";
        else if (code == 0x89)
            key = "8";
        else if (code == 0x8A)
            key = "9";
        else if (code == 0x8B)
            key = "0";
        else if (code == 0x8C)
            key = "-";
        else if (code == 0x8D)
            key = "=";
        else if (code == 0x8E)
            key = "\b";
        else if (code == 0x8F)
            key = "\t";
        else if (code == 0x90)
            key = "q";
        else if (code == 0x91)
            key = "w";
        else if (code == 0x92)
            key = "e";
        else if (code == 0x93)
            key = "r";
        else if (code == 0x94)
            key = "t";
        else if (code == 0x95)
            key = "y";
        else if (code == 0x96)
            key = "u";
        else if (code == 0x97)
            key = "i";
        else if (code == 0x98)
            key = "o";
        else if (code == 0x99)
            key = "p";
        else if (code == 0x9A)
            key = "[";
        else if (code == 0x9B)
            key = "]";
        else if (code == 0x9C)
            key = "\n";
        else if (code == 0xBA)
            shifted = 1;
        else if (code == 0x2A)
            shifted = 1;
        else if (code == 0x36)
            shifted = 1;
        else if (code == 0x9E)
            key = "a";
        else if (code == 0x9F)
            key = "s";
        else if (code == 0xA0)
            key = "d";
        else if (code == 0xA1)
            key = "f";
        else if (code == 0xA2)
            key = "g";
        else if (code == 0xA3)
            key = "h";
        else if (code == 0xA4)
            key = "j";
        else if (code == 0xA5)
            key = "k";
        else if (code == 0xA6)
            key = "l";
        else if (code == 0xA7)
            key = ";";
        else if (code == 0xA8)
            key = "\'";
        else if (code == 0xA8)
            key = "\n";
        else if (code == 0xAC)
            key = "z";
        else if (code == 0xAD)
            key = "x";
        else if (code == 0xAE)
            key = "c";
        else if (code == 0xAF)
            key = "v";
        else if (code == 0xB0)
            key = "b";
        else if (code == 0xB1)
            key = "n";
        else if (code == 0xB2)
            key = "m";
        else if (code == 0xB3)
            key = ",";
        else if (code == 0xB4)
            key = ".";
        else if (code == 0xb5)
            key = "/";
        else if (code == 0xB9)
            key = " ";
        else
            key = "";
    }
    else
    {
        if (code == 0x82)
            key = "!";
        else if (code == 0x83)
            key = "@";
        else if (code == 0x84)
            key = "#";
        else if (code == 0x85)
            key = "$";
        else if (code == 0x86)
            key = "%";
        else if (code == 0x87)
            key = "^";
        else if (code == 0x88)
            key = "&";
        else if (code == 0x89)
            key = "*";
        else if (code == 0x8A)
            key = "(";
        else if (code == 0x8B)
            key = ")";
        else if (code == 0x8C)
            key = "_";
        else if (code == 0x8D)
            key = "+";
        else if (code == 0x8E)
            key = "\b";
        else if (code == 0x8F)
            key = "\t";
        else if (code == 0x90)
            key = "Q";
        else if (code == 0x91)
            key = "W";
        else if (code == 0x92)
            key = "E";
        else if (code == 0x93)
            key = "R";
        else if (code == 0x94)
            key = "T";
        else if (code == 0x95)
            key = "Y";
        else if (code == 0x96)
            key = "U";
        else if (code == 0x97)
            key = "I";
        else if (code == 0x98)
            key = "O";
        else if (code == 0x99)
            key = "P";
        else if (code == 0x9A)
            key = "{";
        else if (code == 0x9B)
            key = "}";
        else if (code == 0x9C)
            key = "\n";
        else if (code == 0xBA)
            shifted = 0;
        else if (code == 0xAA)
            shifted = 0;
        else if (code == 0xB6)
            shifted = 0;
        else if (code == 0x9E)
            key = "A";
        else if (code == 0x9F)
            key = "S";
        else if (code == 0xA0)
            key = "D";
        else if (code == 0xA1)
            key = "F";
        else if (code == 0xA2)
            key = "G";
        else if (code == 0xA3)
            key = "H";
        else if (code == 0xA4)
            key = "J";
        else if (code == 0xA5)
            key = "K";
        else if (code == 0xA6)
            key = "L";
        else if (code == 0xA7)
            key = ":";
        else if (code == 0xA8)
            key = "\"";
        else if (code == 0xA8)
            key = "\n";
        else if (code == 0xAC)
            key = "Z";
        else if (code == 0xAD)
            key = "X";
        else if (code == 0xAE)
            key = "C";
        else if (code == 0xAF)
            key = "V";
        else if (code == 0xB0)
            key = "B";
        else if (code == 0xB1)
            key = "N";
        else if (code == 0xB2)
            key = "M";
        else if (code == 0xB3)
            key = "<";
        else if (code == 0xB4)
            key = ">";
        else if (code == 0xb5)
            key = "?";
        else if (code == 0xB9)
            key = " ";
        else
            key = "";
    }

    if (key != "") {
        if (key == "\n")
        {
            printf("\n\n");
            rc(buff);
            clears(buff);
            bf = 0;
            printf("\n\n> ");
        } else {
            printf(key);
            if (key != "\b")
            {
                for (int z = 0; z < len(key); z++)
                {
                    buff[z+bf] = key[z];
                    bf++;
                }
            }
            else
            {
                if (text_x > 1)
                {
                    buff[bf] = ' ';
                    bf--;
                }
            }

        }
    }
}


void init_keyboard() {
   register_interrupt_handler(IRQ1, scanf);
}
