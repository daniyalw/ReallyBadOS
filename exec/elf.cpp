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
	if(header->e_type != 2)
	{
		Kernel::system_log("File is not executable!\n");
		return 0;
	}
	elf_program_header_t *ph = (elf_program_header_t *)(buf + header->e_phoff);
	for(int i = 0; i < header->e_phnum; i++, ph++)
	{
		switch(ph->p_type)
		 {
		 	case NULL:
		 		break;
		 	case ELF_LOAD:
		 		Kernel::system_log("LOAD: offset:0x%x virtual:0x%x physical:0x%x filesize:0x%x memsize:0x%x\n",
		 				ph->p_offset, ph->p_vaddr, ph->p_paddr, ph->p_filesz, ph->p_memsz);
		 		std::memcpy((char *)ph->p_paddr, (char *)(buf + ph->p_offset), ph->p_filesz);
		 		break;
		 	default:
		 	 Kernel::system_log("Unsupported type!\n");
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
        Kernel::system_log("non-ELF file\n");
        return false;
    }

    return elf_start(buf, argc, argv);
}
