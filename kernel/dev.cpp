int null_read(fs_node_t * node, int offset, int size, char *buf)
{
    UNUSED(buf);
    log::warning("Read was attempted on device '%s'\n", node->path);
    return 1;
}

int null_write(fs_node_t *node, int offset, int size, char *buf)
{
    log::warning("Write was attempted on device '%s'", node->path);
    return 1;
}

int write_serial_(fs_node_t * node, int offset, int size, char *data)
{
    Kernel::serial_write_string(data);
    return 0;
}

int write_info(fs_node_t * node, int offset, int size, char *data)
{
    log::info(data);
    return 0;
}

void init_all_devs()
{
    // sound
    create_file("sound", "/dev", Kernel::sound_input, Kernel::sound_write);

    // keyboard
    create_file("stdin", "/dev", read_keyboard, write_keyboard);

    // ata
    create_file("disk0", "/dev", null_read, null_write);

    // serial
    create_file("serial", "/dev", null_read, write_info);
}
