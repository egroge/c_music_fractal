// pre: include stdint and SDL
#ifndef AUDIO_H
#define AUDIO_H

struct player_data {
    uint8_t *pos;
    uint8_t *wavstart;
    SDL_AudioDeviceID device;
    SDL_AudioSpec wavspec;
    uint32_t length;
    float audioval;    
};

// pre: SDL audio subsystem is already initialised
void play_wav(const char *path, struct player_data *player);

void stop_wav(struct player_data *player);

#endif
