// memory.cpp


void memcpy(unsigned char * source, unsigned char * destination, int nb)
{
    for (int z = 0; z < nb; z++)
    {
        destination[z] = source[z];
    }
}

void memcpy(char * source, char * destination, int nb)
{
    for (int z = 0; z < nb; z++)
    {
        destination[z] = source[z];
    }
}
