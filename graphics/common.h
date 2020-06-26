// Before including file <SDL2/SDL.h> and <GL/gl.h> should be included

#ifndef COMMON_H
#define COMMON_H

#define log_sdl_error(msg)\
        SDL_Log(msg " %s", SDL_GetError());

#define N_VERTICES 4

struct gl_context_win {
    SDL_Window *window;
    SDL_GLContext *context;
};

struct screen_shader {
    struct gl_context_win win;
    GLfloat vertex_buffer_data[3 * N_VERTICES];
    GLuint program;
   	GLuint vertexArrayID;
    GLuint vertexbuffer;
    int iterations;
    struct {
        float x;
        float y;
    } bottom_left;
    struct {
        float x;
        float y;
    } scale;
};

#endif
