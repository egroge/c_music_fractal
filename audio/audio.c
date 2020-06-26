#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "audio.h"

#define my_abs(x) (x < 0 ? -x : x)


// A number to make the audioval appear in a sensible range (about 0 to 5)
#define AUDIO_CONST 15


void myaudiocallback(void *userdata, uint8_t *stream, int stream_length) {
    struct player_data *player = (struct player_data *) userdata;
    if(player->length == 0) {
        // can't call stop_wav sadly. It results in a data race becuase
        // this function is called in another thread I believe. Don't know if i should use locks?
        //stop_wav(player);
        //player->device = 0;
        memset(stream, 0, stream_length); 
        return;
    }
    uint32_t length = ((uint32_t) stream_length) > player->length ? player->length : stream_length;
    
     
    //SDL_memcpy((void *)stream, player->pos, length);
    if(player->wavspec.format == AUDIO_S16 && length % 2 == 0) {
        int16_t *samples = (int16_t *)player->pos;
        uint16_t peak = my_abs(samples[0]);
        bool is_positive = samples[0] >= 0;
        uint32_t npeaks = 1;
        uint32_t sum = 0;
        for(uint32_t i = 0; i < length / 2; i++) {
            ((int16_t*)stream)[i] = samples[i];
            uint16_t displacement = my_abs(samples[i]);
            if(displacement > peak) {
                peak = displacement;
            }
            if(is_positive ^ (samples[i] < 0)) { // could use a xor?
               is_positive = !is_positive;
               sum += peak;
               npeaks++;
               peak = displacement;
            }
        } 
        player->audioval = (float)sum / (float)npeaks / (float)INT16_MAX * AUDIO_CONST;
    } else {
        uint32_t sum = 0;
        for(uint32_t i = 0; i < length; i++) {
            stream[i] = player->pos[i];
            sum += player->pos[i];
        } 
        player->audioval = (float) sum / (float) length / (float) UINT8_MAX;
    }

    player->pos += length;
	player->length -= length;
}

void play_wav(const char *path, struct player_data *player) { 
    uint32_t wav_length;
    
    if(SDL_LoadWAV(path, &player->wavspec, &player->wavstart, &wav_length) == NULL) {
        fputs("unable to load wav file!", stderr);
        return;
    }
    
    player->pos = player->wavstart;
    player->length = wav_length;

    player->wavspec.callback = myaudiocallback;
    player->wavspec.userdata = player;
    
    player->device = SDL_OpenAudioDevice(NULL, 0, &player->wavspec, NULL,
                                         SDL_AUDIO_ALLOW_ANY_CHANGE);
    if(player->device == 0) {
        SDL_FreeWAV(player->wavstart); 
        // call SDL_log
        return;
    }

    SDL_PauseAudioDevice(player->device, 0);
}

void stop_wav(struct player_data *player) {
    if(player->device == 0) {
        // already freed
        return;
    }
    SDL_CloseAudioDevice(player->device); 
    SDL_FreeWAV(player->wavstart);
}

