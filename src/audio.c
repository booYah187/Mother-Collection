#include "audio.h"
#include "config.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>

static Mix_Chunk *tracks[PAGE_COUNT];
static int channels[PAGE_COUNT] = {-1, -1, -1};
static int audio_ready = 0;
static int active_page = 0;

int audio_init(void)
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Audio init failed: %s\n", SDL_GetError());
        return 0;
    }

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) != 0) {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        return 0;
    }

    Mix_AllocateChannels(PAGE_COUNT);

    for (int i = 0; i < PAGE_COUNT; ++i) {
        tracks[i] = Mix_LoadWAV(GAME_PAGES[i].music_path);
        if (!tracks[i]) {
            fprintf(stderr, "Music not loaded (%s): %s\n",
                    GAME_PAGES[i].music_path, Mix_GetError());
        } else {
            channels[i] = Mix_PlayChannel(-1, tracks[i], -1);
            if (channels[i] >= 0) {
                Mix_Volume(channels[i], i == active_page ? MIX_MAX_VOLUME : 0);
            }
        }
    }

    audio_ready = 1;
    return 1;
}

void audio_set_page(int page)
{
    if (!audio_ready || page < 0 || page >= PAGE_COUNT) return;
    active_page = page;

    for (int i = 0; i < PAGE_COUNT; ++i) {
        if (channels[i] >= 0) {
            /* Immediate switch for the first skeleton; later we can add a short ramp. */
            Mix_Volume(channels[i], i == active_page ? MIX_MAX_VOLUME : 0);
        }
    }
}

void audio_shutdown(void)
{
    if (!audio_ready) return;
    Mix_HaltChannel(-1);
    for (int i = 0; i < PAGE_COUNT; ++i) {
        if (tracks[i]) Mix_FreeChunk(tracks[i]);
        tracks[i] = NULL;
        channels[i] = -1;
    }
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    audio_ready = 0;
}
