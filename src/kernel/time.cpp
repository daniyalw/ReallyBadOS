static inline void io_wait()
{
    outb(0x80, 0);
}

void sleep(int sec)
{
    for (int z = 0; z < (10000000 * sec - 5); z++)
        io_wait();
}
