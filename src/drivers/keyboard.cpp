unsigned char last_key;
unsigned char current_key;
unsigned char scanf()
{
    if ((inb(0x64) & 1) != 0)
    {
        current_key = inb(0x60);
        if (last_key == current_key)
            return 0;
        last_key = current_key;
        return current_key;
    }

    return 0;
}

int shifted = 0;
char * returned;
char * useless;

void get_key(unsigned char code)
{
    char * key;
    char * useless;

    if (code == 0)
    {
        returned = "";
        return;
    }

    if (shifted == 0)
    {
        if (code == 0x81)
            key = "esc";
        else if (code == 0x82)
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

    returned = key;
}
