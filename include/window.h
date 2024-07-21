#ifndef WINDOW_H_
#define WINDOW_H_

#include "chip8.h"
#include <SFML/Graphics/Types.h>
#include <SFML/Window/Event.h>

sfRenderWindow *new_window();
void draw(sfRenderWindow *window, unsigned char *display);
void window_event_mapping(sfRenderWindow *window, sfEvent *event, Chip8 *chip8);

#endif
