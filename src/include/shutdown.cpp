void shutdown()
{
    /*
        This works only in newer versions of QEMU
        Look at https://wiki.osdev.org/Shutdown for shutdown on Bochs and VirtualBox
    */
    outw(0x604, 0x2000);
}
