#pragma once

#include <sys/io.h>
#include <sound/sound.h>
#include <filesystem/ramdisk.h>


namespace Kernel {

// https://wiki.osdev.org/PC_Speaker

static void play_sound(uint32_t nFrequence) {
   uint32_t Div;
   uint8_t tmp;

       //Set the PIT to the desired frequency
   Div = 1193180 / nFrequence;
   outb(0x43, 0xb6);
   outb(0x42, (uint8_t) (Div) );
   outb(0x42, (uint8_t) (Div >> 8));

       //And play the sound using the PC speaker
   tmp = inb(0x61);
   if (tmp != (tmp | 3)) {
       outb(0x61, tmp | 3);
   }
}

//make it shutup
static void nosound() {
   uint8_t tmp = inb(0x61) & 0xFC;

   outb(0x61, tmp);
}


char *sound_input(fs_node node, int offset, int size, char *data)
{
    UNUSED(data);
    // not implemented
    return NULL;
}

void sound_write(fs_node node, int offset, int size, char *data)
{
    uint32_t frequency = uint_atoi(data);
    play_sound(frequency);
}

void beep()
{
    FILE *file = fopen("/dev/sound");
    file->write("1000", 0, file->node.size); // there's a difference between 1000 int and 1000 string

    for (int z = 0; z < 4000000; z++)
        io_wait();
    nosound();
}

}
