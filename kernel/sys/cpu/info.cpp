#pragma once
#include <sys/io.h>
#include <cpuid.h>

static int get_model()
{
    int ebx, unused;
    __cpuid(0, unused, ebx, unused, unused);
    return ebx;
}

static inline void __cpuid__(u32 reg, u32 *eax, u32 *ebx, u32 *ecx, u32 *edx)
{
    __asm__ volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

char *  get_vendor()
{
    u32 largestStandardFunc;
    char * vendor;
    __cpuid__(0, &largestStandardFunc, (u32 *)(vendor + 0), (u32 *)(vendor + 8), (u32 *)(vendor + 4));
    vendor[12] = '\0';

    return vendor;
}

bool hypervisor()
{
    uint32_t eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);

    return edx & (1 << 31);
}

char * get_cpu_name()
{
    char * name;
    __cpuid__(0x80000002, (u32 *)(name +  0), (u32 *)(name +  4), (u32 *)(name +  8), (u32 *)(name + 12));
    __cpuid__(0x80000003, (u32 *)(name + 16), (u32 *)(name + 20), (u32 *)(name + 24), (u32 *)(name + 28));
    __cpuid__(0x80000004, (u32 *)(name + 32), (u32 *)(name + 36), (u32 *)(name + 40), (u32 *)(name + 44));

    return name;
}

bool sixtyfour_bit_arch()
{
    u32 eax, ebx, ecx, edx;
    u32 largestExtendedFunc;
    __cpuid__(0x80000000, &largestExtendedFunc, &ebx, &ecx, &edx);

    // Extended Function 0x01 - Extended Feature Bits

    if (largestExtendedFunc >= 0x80000001)
    {
        __cpuid__(0x80000001, &eax, &ebx, &ecx, &edx);

        if (edx & ((1 << 29)))
        {
            return true;
        }
    }

    return false;
}

/* this may not be accurate */
unsigned short get_available_memory()
{
    unsigned short total;
    unsigned char lowmem, highmem;

    Kernel::IO::outb(0x70, 0x30);
    lowmem = Kernel::IO::inb(0x71);
    Kernel::IO::outb(0x70, 0x31);
    highmem = Kernel::IO::inb(0x71);

    total = lowmem | highmem << 8;

    return total;
}
