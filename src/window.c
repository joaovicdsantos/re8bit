#include "window.h"
#include "chip8.h"
#include <SFML/Graphics.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Types.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/WindowBase.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 960
#define BIT_SIZE 32
#define RECTANGLE_SIZE 30

sfRenderWindow *new_window()
{
    sfRenderWindow *window =
        sfRenderWindow_create((sfVideoMode){WINDOW_WIDTH, WINDOW_HEIGHT, BIT_SIZE}, "Re8Bit", sfResize | sfClose, NULL);
    return window;
}

void draw(sfRenderWindow *window, unsigned char *display)
{
    sfRectangleShape *rectangle = sfRectangleShape_create();
    sfRectangleShape_setSize(rectangle, (sfVector2f){RECTANGLE_SIZE, RECTANGLE_SIZE});

    short x = 0, y = 0;
    for (short i = 0; i < DISPLAY_SIZE; i++)
    {
        if (display[i] == 1)
        {
            sfRectangleShape_setPosition(rectangle, (sfVector2f){x, y});
            sfRenderWindow_drawRectangleShape(window, rectangle, NULL);
        }

        x += RECTANGLE_SIZE;
        if (x >= WINDOW_WIDTH)
        {
            x = 0;
            y += RECTANGLE_SIZE;
        }
    }
}

void window_event_mapping(sfRenderWindow *window, sfEvent *event, Chip8 *chip8)
{
    switch (event->type)
    {
    case sfEvtClosed:
        sfRenderWindow_close(window);
        break;
    case sfEvtKeyReleased:
        switch (event->key.code)
        {
        case sfKeyNum1:
            chip8->keys[1] = 0;
            break;
        case sfKeyNum2:
            chip8->keys[2] = 0;
            break;
        case sfKeyNum3:
            chip8->keys[3] = 0;
            break;
        case sfKeyNum4:
            chip8->keys[12] = 0;
            break;
        case sfKeyQ:
            chip8->keys[4] = 0;
            break;
        case sfKeyW:
            chip8->keys[5] = 0;
            break;
        case sfKeyE:
            chip8->keys[6] = 0;
            break;
        case sfKeyR:
            chip8->keys[13] = 0;
            break;
        case sfKeyA:
            chip8->keys[7] = 0;
            break;
        case sfKeyS:
            chip8->keys[8] = 0;
            break;
        case sfKeyD:
            chip8->keys[9] = 0;
            break;
        case sfKeyF:
            chip8->keys[14] = 0;
            break;
        case sfKeyZ:
            chip8->keys[10] = 0;
            break;
        case sfKeyX:
            chip8->keys[0] = 0;
            break;
        case sfKeyC:
            chip8->keys[11] = 0;
            break;
        case sfKeyV:
            chip8->keys[15] = 0;
            break;
        default:
            break;
        }
        break;
    case sfEvtKeyPressed:
        switch (event->key.code)
        {
        case sfKeyNum1:
            chip8->keys[1] = 1;
            break;
        case sfKeyNum2:
            chip8->keys[2] = 1;
            break;
        case sfKeyNum3:
            chip8->keys[3] = 1;
            break;
        case sfKeyNum4:
            chip8->keys[12] = 1;
            break;
        case sfKeyQ:
            chip8->keys[4] = 1;
            break;
        case sfKeyW:
            chip8->keys[5] = 1;
            break;
        case sfKeyE:
            chip8->keys[6] = 1;
            break;
        case sfKeyR:
            chip8->keys[13] = 1;
            break;
        case sfKeyA:
            chip8->keys[7] = 1;
            break;
        case sfKeyS:
            chip8->keys[8] = 1;
            break;
        case sfKeyD:
            chip8->keys[9] = 1;
            break;
        case sfKeyF:
            chip8->keys[14] = 1;
            break;
        case sfKeyZ:
            chip8->keys[10] = 1;
            break;
        case sfKeyX:
            chip8->keys[0] = 1;
            break;
        case sfKeyC:
            chip8->keys[11] = 1;
            break;
        case sfKeyV:
            chip8->keys[15] = 1;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
