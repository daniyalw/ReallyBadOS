unsigned char last_key;
unsigned char scan_code()
{
    unsigned char current_key;
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

char get_key(unsigned char code)
{
    char * key;

    if (code == 0)
        returned = "";

    if (shifted == 0)
    {
        if (code == 0x01 || code == 0x81)
            key = "esc";
        else if (code == 0x02 || code == 0x82)
            key = "1";
        else if (code == 0x03 || code == 0x83)
            key = "2";
        else if (code == 0x04 || code == 0x84)
            key = "3";
        else if (code == 0x05 || code == 0x85)
            key = "4";
        else if (code == 0x06 || code == 0x86)
            key = "5";
        else if (code == 0x07 || code == 0x87)
            key = "6";
        else if (code == 0x08 || code == 0x88)
            key = "7";
        else if (code == 0x09 || code == 0x89)
            key = "8";
        else if (code == 0x0A || code == 0x8A)
            key = "9";
        else if (code == 0x0B || code == 0x8B)
            key = "0";
        else if (code == 0x0C || code == 0x8C)
            key = "-";
        else if (code == 0x0D || code == 0x8D)
            key = "=";
        else if (code == 0x0F || code == 0x8F)
            key = "\t";
        else if (code == 0x10 || code == 0x90)
            key = "q";
        else if (code == 0x11 || code == 0x91)
            key = "w";
        else if (code == 0x12 || code == 0x92)
            key = "e";
        else if (code == 0x13 || code == 0x93)
            key = "r";
        else if (code == 0x14 || code == 0x94)
            key = "t";
        else if (code == 0x15 || code == 0x95)
            key = "y";
        else if (code == 0x16 || code == 0x96)
            key = "u";
        else if (code == 0x17 || code == 0x97)
            key = "i";
        else if (code == 0x18 || code == 0x98)
            key = "o";
        else if (code == 0x19 || code == 0x99)
            key = "p";
        else if (code == 0x1A || code == 0x9A)
            key = "[";
        else if (code == 0x1B || code == 0x9B)
            key = "]";
        else if (code == 0x1C || code == 0x9C)
            key = "\\";
        else if (code == 0x1E || code == 0x9D)
            key = "a";
        else if (code == 0x1F || code == 0x9F)
            key = "s";
        else if (code == 0x20 || code == 0xA0)
            key = "d";
        else if (code == 0x21 || code == 0xA1)
            key = "f";
        else if (code == 0x22 || code == 0xA2)
            key = "g";
        else if (code == 0x23 || code == 0xA3)
            key = "h";
        else if (code == 0x24 || code == 0xA4)
            key = "j";
        else if (code == 0x25 || code == 0xA5)
            key = "k";
        else if (code == 0x26 || code == 0xA6)
            key = "l";
        else if (code == 0x27 || code == 0xA7)
            key = ";";
        else if (code == 0x28 || code == 0xA8)
            key = "\"";
        else if (code == 0x29 || code == 0xA9)
            key = "\n";
        else if (code == 0x2C || code == 0xAC)
            key = "z";
        else if (code == 0x2D || code == 0xAD)
            key = "x";
        else if (code == 0x2E || code == 0xAE)
            key = "c";
        else if (code == 0x2F || code == 0xAF)
            key = "v";
        else if (code == 0x30 || code == 0xB0)
            key = "b";
        else if (code == 0x31 || code == 0xB1)
            key = "n";
        else if (code == 0x32 || code == 0xB2)
            key = "m";
        else if (code == 0x33 || code == 0xB3)
            key = ",";
        else if (code == 0x34 || code == 0xB4)
            key = ".";
        else if (code == 0x35 || code == 0xb5)
            key = "/";
        else if (code == 0x39 || code == 0xB9)
            key = " ";
        else if (code == 0x0E || code == 0x8E)
            key = "BACKSPACE";
    }

    returned = key;
}
