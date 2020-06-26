#ifndef INIT_H
#define INIT_H

#include <stdbool.h>

#define WIN_WIDTH  1000
#define WIN_HEIGHT 750
#define WIN_NAME "Fractal"

#define ITERATIONS 100 
#define BOTTOM_LEFTX -1.5f
#define BOTTOM_LEFTY -1.0f
#define SCALEX 2.0f
#define SCALEY 2.0f

bool window_init(struct gl_context_win *win);

void window_deinit(struct gl_context_win *win);

void screen_shader_init(struct screen_shader *screen);

void screen_shader_deinit(struct screen_shader *screen);
#endif
