#pragma once

#define ELF_LOAD 1

char verify_elf[] = {
    0x7f, 'E', 'L', 'F'
};

typedef int (*call_t)(int argc, char **argv);

typedef struct {
	uint32_t sig;
} elf_priv_data;

typedef struct {
	unsigned char e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} elf_header_t;

typedef struct {
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
} elf_section_header_t;

typedef struct {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} elf_program_header_t;

bool elf_verify(uint8_t *buf);
int elf_start(uint8_t *buf, int argc, char **argv);
int load_app(uint32_t location, int argc, char **argv);
int load_app_from_name(char *name, int argc, char **argv);
int load_app_from_file(FILE *file, int argc, char **argv);
elf_header_t *load_elf_memory(uint8_t *buf);
