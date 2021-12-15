MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 16]                       ; All instructions should be 32-bit.

global mboot                  ; Make 'mboot' accessible from C.
section .multiboot

mboot:
  dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
  dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct

  dd 0 ; skip some flags
  dd 0
  dd 0
  dd 0
  dd 0

  dd 0 ; sets it to graphical mode
  dd 800 ; sets the width
  dd 600 ; sets the height
  dd 32 ; sets the bits per pixel

  dd  mboot                     ; Location of this descriptor
  dd  _start                     ; Kernel entry point (initial EIP).


global _start
extern kernel_main

_start:
  push eax
  push ebx
  call kernel_main                   ; call our main() function.
  cli
  hlt
