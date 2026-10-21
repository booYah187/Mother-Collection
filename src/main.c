#include "launcher.h"

#include <SDL.h>
#include <signal.h>
#include <stdio.h>

static volatile sig_atomic_t quit_requested = 0;

static void signal_handler(int signal_number)
{
    (void)signal_number;
    quit_requested = 1;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    Launcher launcher;
    if (!launcher_init(&launcher)) {
        launcher_shutdown(&launcher);
        return 1;
    }

    Uint32 previous = SDL_GetTicks();
    while (launcher.running && !quit_requested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            launcher_handle_event(&launcher, &event);
        }

        Uint32 now = SDL_GetTicks();
        float dt = (float)(now - previous) / 1000.0f;
        previous = now;
        if (dt > 0.1f) dt = 0.1f;

        launcher_update(&launcher, dt);
        launcher_render(&launcher);
        SDL_Delay(1);
    }

    launcher_shutdown(&launcher);
    return 0;
}
