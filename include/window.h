#ifndef WINDOW_H_
#define WINDOW_H_

#include "chip8.h"
#include <SFML/Graphics/Types.h>
#include <SFML/Window/Event.h>
#include <stdint.h>

sfRenderWindow *new_window();
sfText *new_text();
void draw(sfRenderWindow *window, uint8_t *display, sfText *text, double fps, uint8_t color_index, uint16_t tick);
void window_event_mapping(sfRenderWindow *window, sfEvent *event, Chip8 *chip8, uint8_t *color_index, uint16_t *tick);

#endif
