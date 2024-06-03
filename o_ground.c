#include "SDL_pixels.h"
#include "types.h"
#include "constants.h"
#include "o_ground.h"

static const SDL_Rect rect = {
    .x = 0,
    .y = SCREEN_HEIGHT - GROUND_HEIGHT,
    .w = SCREEN_WIDTH,
    .h = GROUND_HEIGHT,
};

void o_ground_draw (ctx_t * ctx) {
    static SDL_Color ground = { .r = 11, .g = 1, .b = 26, .a = 0 };
    SDL_SetRenderDrawColor(ctx->renderer, ground.r,
                                          ground.g,
                                          ground.b,
                                          ground.a);
    SDL_RenderFillRect(ctx->renderer, &rect);
}
