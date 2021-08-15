# FirstOS
An operating system.

## Building
### macOS
You need `qemu` and `nasm` installed.
Run:
`nasm -f bin boot_sector.asm -o boot_sector.bin`
`qemu-system-x86_64 boot_sector.bin` or `qemu boot_sector.bin`

The OS will load.
