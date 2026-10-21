#include "launcher.h"
#include "audio.h"
#include "launch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static SDL_GameController *controllers[8];

static void open_controllers(void)
{
    int count = SDL_NumJoysticks();
    int slot = 0;
    for (int i = 0; i < count && slot < (int)(sizeof(controllers) / sizeof(controllers[0])); ++i) {
        if (SDL_IsGameController(i)) {
            controllers[slot] = SDL_GameControllerOpen(i);
            if (controllers[slot]) ++slot;
        }
    }
}

static SDL_Texture *load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Optional image not loaded (%s): %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

static void draw_text(Launcher *l, const char *text, int x, int y,
                      SDL_Color color, TTF_Font *font, int centered)
{
    if (!font || !text || !*text) return;
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color,
                                                           (Uint32)(l->width - 24));
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(l->renderer, surface);
    if (texture) {
        SDL_Rect dst = { centered ? (l->width - surface->w) / 2 : x, y,
                         surface->w, surface->h };
        SDL_RenderCopy(l->renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

static SDL_Rect game_rect(Launcher *l)
{
    int w = (int)(l->width * 0.34f);
    int h = (int)(l->height * 0.46f);
    if (w > h * 0.78f) w = (int)(h * 0.78f);
    return (SDL_Rect){(l->width - w) / 2, (int)(l->height * 0.16f), w, h};
}

static SDL_Rect guide_rect(Launcher *l)
{
    int w = (int)(l->width * 0.34f);
    int h = (int)(l->height * 0.09f);
    return (SDL_Rect){(l->width - w) / 2, (int)(l->height * 0.75f), w, h};
}

static SDL_Rect next_rect(Launcher *l)
{
    int s = (int)(l->height * 0.12f);
    if (s < 34) s = 34;
    return (SDL_Rect){l->width - s - (int)(l->width * 0.045f),
                      l->height - s - (int)(l->height * 0.07f), s, s};
}

static void draw_panel(Launcher *l, SDL_Rect rect, int focused, int rounded_unused)
{
    (void)rounded_unused;
    if (focused) {
        SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(l->renderer, 175, 220, 255,
                               (Uint8)(95 + 60 * l->glow_alpha));
        SDL_Rect glow = {rect.x - 5, rect.y - 5, rect.w + 10, rect.h + 10};
        SDL_RenderFillRect(l->renderer, &glow);
    }
    SDL_SetRenderDrawColor(l->renderer, 20, 24, 36, 225);
    SDL_RenderFillRect(l->renderer, &rect);
    SDL_SetRenderDrawColor(l->renderer, focused ? 175 : 224,
                           focused ? 220 : 224, focused ? 255 : 224, 255);
    SDL_RenderDrawRect(l->renderer, &rect);
    SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_NONE);
}

int launcher_init(Launcher *l)
{
    memset(l, 0, sizeof(*l));
    l->page = 0;
    l->focus = FOCUS_GAME;
    l->focus_scale = 1.0f;
    l->glow_alpha = 0.5f;
    l->running = 1;

    int width = 640, height = 480;
    const char *sw = SDL_getenv("SCREEN_WIDTH");
    const char *sh = SDL_getenv("SCREEN_HEIGHT");
    if (sw) { int v = atoi(sw); if (v > 100 && v < 4096) width = v; }
    if (sh) { int v = atoi(sh); if (v > 100 && v < 4096) height = v; }
    l->width = width;
    l->height = height;

    l->window = SDL_CreateWindow(COLLECTION_TITLE, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN);
    if (!l->window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 0;
    }

    l->renderer = SDL_CreateRenderer(l->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!l->renderer) {
        l->renderer = SDL_CreateRenderer(l->window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!l->renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 0;
    }

    SDL_RenderSetLogicalSize(l->renderer, width, height);
    open_controllers();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (TTF_Init() == 0) {
        const char *font_paths[] = {
            "assets/fonts/launcher.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
        };
        for (size_t i = 0; i < sizeof(font_paths) / sizeof(font_paths[0]); ++i) {
            if (!l->font) l->font = TTF_OpenFont(font_paths[i], 24);
            if (!l->small_font) l->small_font = TTF_OpenFont(font_paths[i], 16);
        }
        if (!l->font) fprintf(stderr, "No usable font found; add assets/fonts/launcher.ttf\n");
    }

    for (int i = 0; i < PAGE_COUNT; ++i) {
        l->backgrounds[i] = load_texture(l->renderer, GAME_PAGES[i].background_path);
        l->boxarts[i] = load_texture(l->renderer, GAME_PAGES[i].boxart_path);
    }

    audio_init();
    audio_set_page(l->page);
    return 1;
}

void launcher_shutdown(Launcher *l)
{
    audio_shutdown();
    for (size_t i = 0; i < sizeof(controllers) / sizeof(controllers[0]); ++i) {
        if (controllers[i]) SDL_GameControllerClose(controllers[i]);
        controllers[i] = NULL;
    }
    for (int i = 0; i < PAGE_COUNT; ++i) {
        if (l->backgrounds[i]) SDL_DestroyTexture(l->backgrounds[i]);
        if (l->boxarts[i]) SDL_DestroyTexture(l->boxarts[i]);
    }
    if (l->font) TTF_CloseFont(l->font);
    if (l->small_font) TTF_CloseFont(l->small_font);
    TTF_Quit();
    IMG_Quit();
    if (l->renderer) SDL_DestroyRenderer(l->renderer);
    if (l->window) SDL_DestroyWindow(l->window);
    SDL_Quit();
}

void launcher_show_message(Launcher *l, const char *message, Uint32 duration_ms)
{
    snprintf(l->message, sizeof(l->message), "%s", message ? message : "");
    l->message_until = SDL_GetTicks() + duration_ms;
}

static void change_page(Launcher *l, int delta)
{
    l->page = (l->page + delta + PAGE_COUNT) % PAGE_COUNT;
    l->focus = FOCUS_GAME;
    audio_set_page(l->page);
}

void launcher_handle_event(Launcher *l, const SDL_Event *event)
{
    if (event->type == SDL_QUIT) {
        l->running = 0;
        return;
    }

    if (event->type == SDL_KEYDOWN && !event->key.repeat) {
        switch (event->key.keysym.sym) {
            case SDLK_LEFT: case SDLK_UP:
                l->focus = (FocusItem)((l->focus + FOCUS_COUNT - 1) % FOCUS_COUNT);
                break;
            case SDLK_RIGHT: case SDLK_DOWN:
                l->focus = (FocusItem)((l->focus + 1) % FOCUS_COUNT);
                break;
            case SDLK_RETURN: case SDLK_SPACE: case SDLK_z:
                launcher_activate(l);
                break;
            case SDLK_PAGEUP:
                change_page(l, -1);
                break;
            case SDLK_PAGEDOWN:
                change_page(l, 1);
                break;
            case SDLK_ESCAPE:
                break;
            default: break;
        }
    }

    if (event->type == SDL_CONTROLLERBUTTONDOWN) {
        switch (event->cbutton.button) {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                l->focus = (FocusItem)((l->focus + FOCUS_COUNT - 1) % FOCUS_COUNT);
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                l->focus = (FocusItem)((l->focus + 1) % FOCUS_COUNT);
                break;
            case SDL_CONTROLLER_BUTTON_A:
                launcher_activate(l);
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                change_page(l, -1);
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                change_page(l, 1);
                break;
            default: break;
        }
    }
}

void launcher_update(Launcher *l, float dt)
{
    float target = 1.0f;
    l->focus_scale += (target - l->focus_scale) * (dt * 8.0f);
    l->glow_alpha = 0.5f + 0.5f * sinf((float)SDL_GetTicks() / 260.0f);
    if (l->message[0] && SDL_TICKS_PASSED(SDL_GetTicks(), l->message_until)) {
        l->message[0] = '\0';
    }
}

static void render_art(Launcher *l, SDL_Texture *texture, SDL_Rect rect, int focused)
{
    if (focused) {
        float scale = 1.035f;
        int dw = (int)(rect.w * scale), dh = (int)(rect.h * scale);
        rect.x -= (dw - rect.w) / 2;
        rect.y -= (dh - rect.h) / 2;
        rect.w = dw;
        rect.h = dh;
    }

    SDL_Rect dst = rect;
    int has_texture = 0;

    if (texture) {
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

        if (tw > 0 && th > 0) {
            float scale = fminf((float)rect.w / (float)tw,
                                (float)rect.h / (float)th);

            dst = (SDL_Rect){
                (int)(rect.x + (rect.w - tw * scale) / 2),
                (int)(rect.y + (rect.h - th * scale) / 2),
                (int)(tw * scale),
                (int)(th * scale)
            };
            has_texture = 1;
        }
    }

    /*
     * Glow follows the actual rendered artwork dimensions instead of
     * the portrait-shaped boxart container.
     */
    if (focused) {
        SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(l->renderer, 175, 220, 255,
                               (Uint8)(80 + 70 * l->glow_alpha));

        SDL_Rect glow = {
            dst.x - 5,
            dst.y - 5,
            dst.w + 10,
            dst.h + 10
        };
        SDL_RenderFillRect(l->renderer, &glow);
    }

    if (has_texture) {
        SDL_SetRenderDrawColor(l->renderer, 255, 255, 255, 255);
        SDL_RenderCopy(l->renderer, texture, NULL, &dst);
    } else {
        SDL_SetRenderDrawColor(l->renderer, 36, 48, 64, 255);
        SDL_RenderFillRect(l->renderer, &rect);
        SDL_SetRenderDrawColor(l->renderer, 175, 220, 255, 255);
        SDL_RenderDrawRect(l->renderer, &rect);
        draw_text(l, GAME_PAGES[l->page].title, rect.x + 8, rect.y + rect.h / 2 - 12,
                  (SDL_Color){255,255,255,255}, l->font, 1);
    }

    SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_NONE);
}

void launcher_render(Launcher *l)
{
    SDL_SetRenderDrawColor(l->renderer, 20, 29, 43, 255);
    SDL_RenderClear(l->renderer);

    if (l->backgrounds[l->page]) {
        SDL_RenderCopy(l->renderer, l->backgrounds[l->page], NULL, NULL);
    } else {
        /* Subtle default gradient substitute until user art is added. */
        SDL_SetRenderDrawColor(l->renderer, 28 + l->page * 12, 43 + l->page * 5, 62 + l->page * 10, 255);
        SDL_RenderClear(l->renderer);
    }

    SDL_Color cream = {255, 244, 213, 255};
    draw_text(l, COLLECTION_TITLE, 12, 12, cream, l->small_font, 0);

    char page_label[32];
    snprintf(page_label, sizeof(page_label), "%02d / %02d", l->page + 1, PAGE_COUNT);
    draw_text(l, page_label, l->width - 82, 12, cream, l->small_font, 0);

    SDL_Rect art = game_rect(l);
    render_art(l, l->boxarts[l->page], art, l->focus == FOCUS_GAME);
    draw_text(l, GAME_PAGES[l->page].title, 0, art.y + art.h + 7, cream, l->font, 1);

    SDL_Rect guide = guide_rect(l);
    draw_panel(l, guide, l->focus == FOCUS_GUIDE, 0);
    draw_text(l, "PLAYER'S GUIDE", guide.x, guide.y + guide.h / 2 - 9, cream, l->small_font, 1);

    SDL_Rect next = next_rect(l);
    draw_panel(l, next, l->focus == FOCUS_NEXT, 0);
    SDL_Point points[3] = {
        {next.x + next.w / 3, next.y + next.h / 4},
        {next.x + next.w * 2 / 3, next.y + next.h / 2},
        {next.x + next.w / 3, next.y + next.h * 3 / 4}
    };
    SDL_SetRenderDrawColor(l->renderer, 255, 244, 213, 255);
    SDL_RenderDrawLines(l->renderer, points, 3);

    if (l->message[0]) {
        SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(l->renderer, 0, 0, 0, 210);
        SDL_Rect overlay = {12, l->height / 2 - 58, l->width - 24, 116};
        SDL_RenderFillRect(l->renderer, &overlay);
        SDL_SetRenderDrawColor(l->renderer, 255, 220, 140, 255);
        SDL_RenderDrawRect(l->renderer, &overlay);
        draw_text(l, l->message, 24, overlay.y + 18, cream, l->small_font, 1);
        SDL_SetRenderDrawBlendMode(l->renderer, SDL_BLENDMODE_NONE);
    }

    SDL_RenderPresent(l->renderer);
}

int launcher_activate(Launcher *l)
{
    if (l->focus == FOCUS_GAME) return launch_game(l, l->page);
    if (l->focus == FOCUS_GUIDE) return launch_guide(l, l->page);
    change_page(l, 1);
    return 0;
}
