#include <exec/elf.h>

/* returns true if true elf, otherwise false */
bool elf_verify(uint8_t *buf) {
    for (int z = 0; z < 4; z++)
        if (buf[z] != verify_elf[z])
            return false;

    return true;
}

elf_header_t *load_elf_memory(uint8_t *buf) {
    elf_header_t *header = (elf_header_t *)buf;

    if (header->e_type != 2) {
        log::warning("File is not executable!");
        return 0;
    }

    elf_program_header_t *ph = (elf_program_header_t *)(buf + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++, ph++) {
        switch(ph->p_type) {
            case NULL:
                break;
            case ELF_LOAD:
                log::info("LOADING EXECUTABLE: offset:0x%x virtual:0x%x physical:0x%x filesize:0x%x memsize:0x%x",
                        ph->p_offset, ph->p_vaddr, ph->p_paddr, ph->p_filesz, ph->p_memsz);
                memcpy((char *)ph->p_paddr, (char *)(buf + ph->p_offset), ph->p_filesz);
                break;
            default:
             log::warning("Unsupported type!");
             return 0;
         }
    }

    return header;
}

int elf_start(uint8_t *buf, int argc, char **argv) {
    elf_header_t *header = load_elf_memory(buf);

    call_t func = (call_t)header->e_entry;
    int result = func(argc, argv);

    return result;
}

int load_app(uint32_t location, int argc, char **argv) {
    uint8_t *buf = (uint8_t *)location;

    if (!elf_verify(buf)) {
        log::warning("non-ELF file\n");
        return 1; // if we return 0 it'll be taken as success
    }

    return elf_start(buf, argc, argv);
}

int load_app_from_name(char *name, int argc, char **argv) {
    FILE *file = fopen(name, "r");

    if (file == NULL) {
        log::warning("ELF file not found.\n");
        return 1;
    }

    uint8_t *buf = (uint8_t *)file->node->contents;

    if (!elf_verify(buf)) {
        log::warning("Invalid ELF file.\n");
        return 1;
    }

    return elf_start(buf, argc, argv);
}

// does not handle arguments
// can do things like echo -> /bin/echo
char *exec_path_from_name(char *name) {
    char *path;
    get(path, "/bin/%s.o", name);
    FILE *file = fopen(path, "r");

    if (!file) {
        get(path, "/usr/bin/%s.o", name);
        file = fopen(path, "r");

        if (!file) {
            return NULL;
        }
    }

    return path;
}

int load_app_from_file(FILE *file, int argc, char **argv) {
    uint8_t *buf = (uint8_t *)file->node->contents;

    if (!elf_verify(buf)) {
        log::warning("Invalid ELF file.\n");
        return 1;
    }

    return elf_start(buf, argc, argv);
    return 0;
}

int execv(char *path, char **argv) {
    int argc = 0;

    while (true) {
        if (argv[argc] == NULL) {
            break;
        }

        argc++;
    }

    return load_app_from_name(path, argc, argv);
}

int execvp(char *file, char **argv) {
    return execv(file, argv);
}
