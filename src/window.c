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
