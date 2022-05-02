#pragma once

namespace System {

static int get_model();
static inline void __cpuid__(u32 reg, u32 *eax, u32 *ebx, u32 *ecx, u32 *edx);
char * get_vendor();
bool hypervisor();
char * get_cpu_name(char *name);
bool sixtyfour_bit_arch();
unsigned short get_available_memory();

}
