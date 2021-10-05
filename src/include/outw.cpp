void outw(unsigned short port, unsigned short data)
{
    asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}
