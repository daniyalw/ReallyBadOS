typedef struct
{
   u16 limit_low;           // The lower 16 bits of the limit.
   u16 base_low;            // The lower 16 bits of the base.
   u8  base_middle;         // The next 8 bits of the base.
   u8  access;              // Access flags, determine what ring this segment can be used in.
   u8  granularity;
   u8  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;


typedef struct
{
   u16 limit;               // The upper 16 bits of all selector limits.
   u32 base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed)) gdt_ptr_t;

extern "C" {
    extern void gdt_flush(u32 push);
}

// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void init_descriptor_tables();
