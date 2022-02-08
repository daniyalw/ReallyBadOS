extern "C" int atoi(char * str)
{
    int num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}
