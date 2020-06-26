#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "common.h"
#include "transform.h"

#define ITER_STEP 10
#define ZOOM_RATE 0.05f

void decrease_iter(struct screen_shader *screen) {
    screen->iterations -= ITER_STEP;
}

void increase_iter(struct screen_shader *screen) {
    screen->iterations += ITER_STEP;
}

void zoomout(struct screen_shader *screen) {
    screen->scale.x *= (1 + ZOOM_RATE); 
    screen->scale.y *= (1 + ZOOM_RATE); 
    screen->bottom_left.x -= screen->scale.x * ZOOM_RATE * 0.5f;
    screen->bottom_left.y -= screen->scale.y * ZOOM_RATE * 0.5f;
}
void zoomin(struct screen_shader *screen) {
    screen->scale.x *= (1 - ZOOM_RATE); 
    screen->scale.y *= (1 - ZOOM_RATE); 
    screen->bottom_left.x += screen->scale.x * ZOOM_RATE * 0.5f;
    screen->bottom_left.y += screen->scale.y * ZOOM_RATE * 0.5f;
}

void moveleft(struct screen_shader *screen) {
    screen->bottom_left.x -= screen->scale.x * ZOOM_RATE;
}
void moveright(struct screen_shader *screen) {
    screen->bottom_left.x += screen->scale.x * ZOOM_RATE;
}
void moveup(struct screen_shader *screen) {
    screen->bottom_left.y += screen->scale.y * ZOOM_RATE;
}
void movedown(struct screen_shader *screen) {
    screen->bottom_left.y -= screen->scale.y * ZOOM_RATE;
}
