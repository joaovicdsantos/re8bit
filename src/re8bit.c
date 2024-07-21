#include "chip8.h"
#include "window.h"
#include <SFML/Config.h>
#include <SFML/Graphics.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/System/Clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    Chip8 chip8;
    sfRenderWindow *window = NULL;
    srand(time(NULL));

    if (argc < 2)
    {
        printf("Invalid number of arguments.\nUsage: ./chip8 <rom_file>\n");
        exit(1);
    }

    chip8 = new_chip8();
    window = new_window();

    load_rom(&chip8, argv[1]);

    // sfClock *clock = sfClock_create();
    sfRenderWindow_setFramerateLimit(window, 800);
    while (sfRenderWindow_isOpen(window))
    {
        // float current_time = sfClock_restart(clock).microseconds;
        // float fps = 1.0f / current_time * 1000000;
        // printf("fps: %.2f\n", fps);

        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
            window_event_mapping(window, &event, &chip8);

        emulate_cycle(&chip8);

        sfRenderWindow_clear(window, sfBlack);
        draw(window, chip8.display);
        sfRenderWindow_display(window);
    }

    return 0;
}
