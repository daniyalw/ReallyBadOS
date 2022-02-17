char *null_read(char *buf)
{
    UNUSED(buf);
    log::warning("Read was attempted on /dev/serial\n");
    return NULL;
}

void write_serial_(char *data)
{
    Kernel::serial_write_string(data);
}

void write_info(char *data)
{
    log::info(data);
}

void init_all_devs()
{
    // sound
    create_file("sound", "/dev/", Kernel::sound_input, Kernel::sound_write);

    // keyboard
    create_file("stdin", "/dev/", read_keyboard, write_keyboard);
    create_file("getch", "/dev/", read_getch, write_keyboard);

    // ata
    create_file("ata", "/dev/", fs_ata_read, fs_ata_write);

    // serial
    create_file("serial", "/dev/", null_read, write_info);
}
