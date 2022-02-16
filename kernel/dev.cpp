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
    create_file("serial", "/dev/", Kernel::get_log, Kernel::log);
}
