#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid number of arguments.\nUsage: ./chip8 <rom_file>\n");
        exit(1);
    }
    char *rom_path = argv[1];

    Chip8 chip8 = new_chip8();

    load_rom(&chip8, rom_path);

    int c = 0;
    while (true)
    {
        emulate_cycle(&chip8);

        if (chip8.display_flag)
            draw(chip8.display);

        c++;
        if (c > 300)
            break;
    }

    return 0;
}
