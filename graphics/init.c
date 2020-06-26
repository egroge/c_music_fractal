#include <GL/glew.h>
#include <GL/gl.h>
#include "SDL2/SDL.h"
#include <stdbool.h>
#include "common.h"
#include "init.h"

bool window_init(struct gl_context_win *win) {
    // can change window pos later
    win->window = SDL_CreateWindow(WIN_NAME, 0, 0, WIN_WIDTH, WIN_HEIGHT, 
                                   SDL_WINDOW_OPENGL);
    if(win->window == NULL) {
        log_sdl_error("Unable to create window");
        return false;
    } 

    win->context = SDL_GL_CreateContext(win->window);
                  
    if(win->context == NULL) {
        log_sdl_error("Unable to create context");
        return false;
    }
    return true;
}

void window_deinit(struct gl_context_win *win) {
    SDL_GL_DeleteContext(win->context);
    SDL_DestroyWindow(win->window);
}

void screen_shader_init(struct screen_shader *screen) {
    screen->program = glCreateProgram();

    glGenVertexArrays(1, &screen->vertexArrayID);
	glBindVertexArray(screen->vertexArrayID);

    // Each set of 3 floats is a point in 3d space. There are 4 points
    // one for each corner of the screen. The first three points
    // define a triangle from upper left, lower left and lower right.
    // Points 2 - 4 define a triangle from upper left, upper right, and lower right
    // Together, when drawn, the entire screen is covered
    screen->vertex_buffer_data[0] =  -1.0f; // Lower left
    screen->vertex_buffer_data[1] =  -1.0f;
    screen->vertex_buffer_data[2] =   0.0f;
    screen->vertex_buffer_data[3] =   1.0f; // Lower right
    screen->vertex_buffer_data[4] =  -1.0f;
    screen->vertex_buffer_data[5] =   0.0f;
    screen->vertex_buffer_data[6] =  -1.0f; // Upper Left
    screen->vertex_buffer_data[7] =   1.0f;
    screen->vertex_buffer_data[8] =   0.0f;
    screen->vertex_buffer_data[9] =   1.0f; // Upper right
    screen->vertex_buffer_data[10] =  1.0f;
    screen->vertex_buffer_data[11] =  0.0f;

    glGenBuffers(1, &screen->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screen->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen->vertex_buffer_data), screen->vertex_buffer_data, GL_STATIC_DRAW);

    screen->iterations = ITERATIONS;
    screen->bottom_left.x = BOTTOM_LEFTX;
    screen->bottom_left.y = BOTTOM_LEFTY;
    screen->scale.x = SCALEX;
    screen->scale.y = SCALEY;
}

void screen_shader_deinit(struct screen_shader *screen) {
    glDeleteBuffers(1, &screen->vertexbuffer);
	glDeleteVertexArrays(1, &screen->vertexArrayID);
	glDeleteProgram(screen->program);
}
