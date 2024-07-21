#ifndef WINDOW_H_
#define WINDOW_H_

#include <SFML/Graphics/Types.h>

sfRenderWindow *new_window();
void draw(sfRenderWindow *window, unsigned char *display);

#endif
