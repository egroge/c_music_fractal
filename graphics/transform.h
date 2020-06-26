#ifndef TRANSFORM_H
#define TRANSFORM_H

void decrease_iter(struct screen_shader *screen);

void increase_iter(struct screen_shader *screen);

void zoomout(struct screen_shader *screen);
    
void zoomin(struct screen_shader *screen);

void moveleft(struct screen_shader *screen);

void moveright(struct screen_shader *screen);

void moveup(struct screen_shader *screen); 

void movedown(struct screen_shader *screen);
#endif
