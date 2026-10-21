#ifndef COLLECTION_LAUNCHER_H
#define COLLECTION_LAUNCHER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "config.h"

typedef enum {
    FOCUS_GAME = 0,
    FOCUS_GUIDE = 1,
    FOCUS_NEXT = 2,
    FOCUS_COUNT = 3
} FocusItem;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *backgrounds[PAGE_COUNT];
    SDL_Texture *boxarts[PAGE_COUNT];
    TTF_Font *font;
    TTF_Font *small_font;
    int width;
    int height;
    int page;
    FocusItem focus;
    float focus_scale;
    float glow_alpha;
    char message[512];
    Uint32 message_until;
    int running;
} Launcher;

int launcher_init(Launcher *launcher);
void launcher_shutdown(Launcher *launcher);
void launcher_handle_event(Launcher *launcher, const SDL_Event *event);
void launcher_update(Launcher *launcher, float dt);
void launcher_render(Launcher *launcher);
void launcher_show_message(Launcher *launcher, const char *message, Uint32 duration_ms);
int launcher_activate(Launcher *launcher);

#endif
