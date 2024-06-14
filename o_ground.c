#include <assert.h>
#include "SDL_render.h"
#include "SDL_rect.h"
#include "types.h"
#include "o_ground.h"
#include "o_scene.h"

void o_ground_draw (ctx_t * ctx, SDL_Renderer * renderer) {
    SDL_Rect tgt = sim2tgt(ctx->scene, ctx->ground.sim);
    SDL_SetRenderDrawColor(renderer, ctx->colors.ground.r,
                                     ctx->colors.ground.g,
                                     ctx->colors.ground.b,
                                     ctx->colors.ground.a);
    SDL_RenderFillRect(renderer, &tgt);
}

void o_ground_init (ctx_t * ctx) {
    assert(ctx->scene.tgt.w != 0 && "scene needs to be initialized before ground");
    float h = 100;
    ctx->ground.sim = (SDL_FRect) {
        .h = h,
        .w = ctx->scene.sim.w,
        .x = 0,
        .y = ctx->scene.sim.h - h,
    };
}
