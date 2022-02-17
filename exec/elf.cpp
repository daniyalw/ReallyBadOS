#include <exec/elf.h>

/* returns true if true elf, otherwise false */
bool elf_verify(uint8_t *buf)
{
    for (int z = 0; z < 4; z++)
        if (buf[z] != verify_elf[z])
            return false;

    return true;
}

int elf_start(uint8_t *buf, int argc, char **argv)
{
	elf_header_t *header = (elf_header_t *)buf;

	if (header->e_type != 2)
	{
		log::warning("File is not executable!");
		return 0;
	}

	elf_program_header_t *ph = (elf_program_header_t *)(buf + header->e_phoff);

	for (int i = 0; i < header->e_phnum; i++, ph++)
	{
		switch(ph->p_type)
		 {
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

    call_t func = (call_t)header->e_entry;
    int result = func(argc, argv);

    return result;
}

int load_app(uint32_t location, int argc, char **argv)
{
    uint8_t *buf = (uint8_t *)location;

    if (!elf_verify(buf))
    {
        log::warning("non-ELF file\n");
        return 1; // if we return 0 it'll be taken as success
    }

    return elf_start(buf, argc, argv);
}

int load_app_from_name(char *name, int argc, char **argv)
{
    FILE *file = fopen(name);

    if (file->null)
    {
        log::warning("ELF file not found.\n");
        return 1;
    }

    uint8_t *buf = (uint8_t *)file->node.contents;

    if (!elf_verify(buf))
    {
        log::warning("Invalid ELF file.\n");
        return 1;
    }

    return elf_start(buf, argc, argv);
}

int load_app_from_file(FILE *file, int argc, char **argv)
{
    uint8_t *buf = (uint8_t *)file->node.contents;

    if (!elf_verify(buf))
    {
        log::warning("Invalid ELF file.\n");
        return 1;
    }

    return elf_start(buf, argc, argv);
}
