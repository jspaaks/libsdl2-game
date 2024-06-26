#include "renderer.h"
#include "deinit.h"
#include "SDL_error.h"
#include "SDL_hints.h"
#include "SDL_render.h"
#include "types.h"

void renderer_deinit (SDL_Renderer ** renderer) {
    if (renderer == NULL) return;
    SDL_DestroyRenderer(*renderer);
    *renderer = NULL;
}

SDL_Renderer * renderer_init (SDL_Window * window) {
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        SDL_Log("Error initializing renderer: %s\n", SDL_GetError());
        deinit();
    }
    return renderer;
}
