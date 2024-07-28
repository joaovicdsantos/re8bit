#include "beep.h"
#include "chip8.h"
#include "window.h"
#include <SFML/Audio.h>
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundBuffer.h>
#include <SFML/Audio/Types.h>
#include <SFML/Config.h>
#include <SFML/Graphics.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Types.h>
#include <SFML/System/Clock.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _60HZ_MICROSECONDS 16666.67

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid number of arguments.\nUsage: ./chip8 <rom_file>\n");
        exit(1);
    }

    Chip8 chip8;
    sfRenderWindow *window = NULL;
    sfSound *beep_sound = NULL;
    sfText *info_text = NULL;
    uint8_t color_index = 0;
    uint16_t tick = 520;

    srand(time(NULL));

    chip8 = new_chip8();
    load_rom(&chip8, argv[1]);

    window = new_window();
    beep_sound = new_beep();
    info_text = new_text();

    sfClock *fps_clock = sfClock_create();
    sfClock *tick_clock = sfClock_create();

    while (sfRenderWindow_isOpen(window))
    {
        sfRenderWindow_setFramerateLimit(window, tick);

        float current_time = sfClock_restart(fps_clock).microseconds;
        float fps = 1.0f / current_time * 1000000;

        // Decrease timers 60hz
        if (sfClock_getElapsedTime(tick_clock).microseconds >= _60HZ_MICROSECONDS)
        {
            sfClock_restart(tick_clock);
            decrease_timers(&chip8);
        }

        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
            window_event_mapping(window, &event, &chip8, &color_index, &tick);

        emulate_cycle(&chip8);

        if (chip8.sound_timer == 1 && !chip8.muted)
            sfSound_play(beep_sound);

        sfRenderWindow_clear(window, sfBlack);
        draw(window, chip8.display, info_text, fps, color_index, tick);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
    sfText_destroy(info_text);
    sfSound_destroy(beep_sound);
    sfClock_destroy(fps_clock);
    sfClock_destroy(tick_clock);

    return 0;
}
