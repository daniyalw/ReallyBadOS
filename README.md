# FirstOS
An operating system.

NOTE: I have no clue how to build the kernel, and I can't figure out how to run `C` functions in Assembly. Forgive me, I added the kernel but I will do my best to add it to the OS.

## Building
### macOS
You need `qemu` and `nasm` installed.
Run:
`nasm -f bin boot_sector.asm -o boot_sector.bin`
`qemu-system-x86_64 boot_sector.bin` or `qemu boot_sector.bin`

The OS will load.
