#include "chip8.h"
#include "window.h"
#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Chip8 chip8;
    sfRenderWindow *window = NULL;

    if (argc < 2)
    {
        printf("Invalid number of arguments.\nUsage: ./chip8 <rom_file>\n");
        exit(1);
    }

    chip8 = new_chip8();
    window = new_window();

    load_rom(&chip8, argv[1]);

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        emulate_cycle(&chip8);

        sfRenderWindow_clear(window, sfBlack);
        draw(window, chip8.display);
        sfRenderWindow_display(window);
    }

    return 0;
}
