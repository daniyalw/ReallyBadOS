// from forum.osdev.org
dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLP_TYPa;
word SLP_TYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;

uint8_t lapic_ids[256]={0}; // CPU core Local APIC IDs
uint8_t numcore=0;          // number of cores detected
uint64_t lapic_ptr=0;       // pointer to the Local APIC MMIO registers
uint64_t ioapic_ptr=0;      // pointer to the IO APIC MMIO registers

struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};

struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};
