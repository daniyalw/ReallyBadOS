extern "C" int strlen(char * text)
{
    int length = 0;

    while (text[length]) length++;

    return length;
}
