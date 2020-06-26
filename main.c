#include <stdbool.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "graphics/common.h"
#include "graphics/init.h"
#include "graphics/loadshader.h"
#include "graphics/transform.h"
#include "audio/audio.h"


void handle_input(SDL_Event *e, bool *quit, struct screen_shader *screen) {
    while (SDL_PollEvent(e) != 0) {
        if (e->type == SDL_QUIT) {
            *quit = true;
        } else if (e->type == SDL_KEYDOWN) {
            switch (e->key.keysym.sym) {
                case SDLK_ESCAPE:
                    *quit = true;
                    break;
                case SDLK_EQUALS:
                    increase_iter(screen);
                    break;
                case SDLK_MINUS:
                    decrease_iter(screen);
                    break;
                case SDLK_w:
                    zoomin(screen);
                    break;
                case SDLK_q:
                    zoomout(screen);
                    break;
                case SDLK_h:
                case SDLK_LEFT:
                    moveleft(screen);
                    break;
                case SDLK_j:
                case SDLK_DOWN:
                    movedown(screen);
                    break;
                case SDLK_k:
                case SDLK_UP:
                    moveup(screen);
                    break;
                case SDLK_l:
                case SDLK_RIGHT:
                    moveright(screen);
                    break;
            }
        }
    }
}

void set_uniforms(struct screen_shader *screen, int n, float audioval) {
    GLint bottom_left = glGetUniformLocation(screen->program, "bottom_left");
    GLint scale = glGetUniformLocation(screen->program, "scale");
    GLint iterations = glGetUniformLocation(screen->program, "iterations");
    GLint audioval_loc = glGetUniformLocation(screen->program, "audioval");
    // This is for when we load the power n file
    // GLint n_address = glGetUniformLocation(screen->program, "n");
    
    glUseProgram(screen->program);
    glUniform2f(bottom_left, screen->bottom_left.x, screen->bottom_left.y);
    glUniform2f(scale, screen->scale.x, screen->scale.y);
    glUniform1i(iterations, screen->iterations);
    glUniform1f(audioval_loc, audioval);

    // glUniform1i(n_address, n);
}

bool interpret_cmd_line_inputs(int argc, char **argv, char **shader, char **song) {
    // If no arguments are specified, just render the normal mandlebrot
    if (argc == 1) {
        *shader = "graphics/shaders/mbrot1.glfs";
        song = NULL;
        return false;
    }

    // We assume the passed in files are correct
    *shader = argv[1];
    if (argc < 3) {
        *song = NULL;
        return false;
    } else {
        *song = argv[2];
        return true;
    }
}

// TODO add that the program stops when the command line arguments are bad
int main(int argc, char **argv) {
    char *shader = NULL;
    char *song = NULL;

    bool audio_enabled = interpret_cmd_line_inputs(argc, argv, &shader, &song);
    printf("You want to use the shader: %s\n", shader);
    printf("You want music: %s\n", audio_enabled ? song : "false");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        log_sdl_error("Unable to initialise SDL");
    }
    atexit(SDL_Quit);
   
    struct player_data player;
    if (audio_enabled) {
        play_wav(song, &player); 
    }

    struct screen_shader screen;
    if (!window_init(&screen.win)) {
        return EXIT_FAILURE;
    }
    glewInit();

    screen_shader_init(&screen);
    screen.program = load_program(shader, "graphics/shaders/vertex.glvs");
   
    // Width and height never change at runtime 
    glUseProgram(screen.program);
    GLint width_address = glGetUniformLocation(screen.program, "width");
    GLint height_address = glGetUniformLocation(screen.program, "height");
    glUniform1i(width_address, WIN_WIDTH);
    glUniform1i(height_address, WIN_HEIGHT);
    
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        handle_input(&e, &quit, &screen);
        set_uniforms(&screen, 2, audio_enabled ? player.audioval : 1);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, screen.vertexbuffer);
        glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but
                // must match the layout in the shader.
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0,        // stride
                (void *) 0 // array buffer offset
                );

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 1, 3);
        glDisableVertexAttribArray(0);

        // Swap buffers
        SDL_GL_SwapWindow(screen.win.window);
    }

    if (audio_enabled) { 
        stop_wav(&player);
    }
    
    screen_shader_deinit(&screen); 
    window_deinit(&screen.win);
    return EXIT_SUCCESS;
}
