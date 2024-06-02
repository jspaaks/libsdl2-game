#include "o_flash.h"
#include "types.h"
#include <SDL_rect.h>
#include <stdio.h>

void o_flash_draw (ctx_t * ctx) {
    if (ctx->flash.show) {
        SDL_RenderCopyEx(ctx->renderer, ctx->spritesheet, &ctx->flash.src,
                                                          &ctx->flash.tgt,
                                                          ctx->barrel.angle,
                                                          &ctx->flash.pivot_offset,
                                                          SDL_FLIP_NONE);
    }
}

ctx_t * o_flash_init (ctx_t * ctx) {
    float h = 21;
    float w = 30;
    float loffset = (ctx->barrel.tgt.w + 2);
    float x = ctx->barrel.pivot.x + loffset;
    float y = ctx->barrel.pivot.y - (h - 1) / 2;
    ctx->flash = (flash_t) {
        .pivot_offset = (SDL_Point) {
            .x = -1 * loffset,
            .y = (h - 1) / 2,
        },
        .show = true,
        .sim = (SDL_FRect) {
            .h = h,
            .w = w,
            .x = x,
            .y = y,
        },
        .src = (SDL_Rect) {
            .h = h,
            .w = w,
            .x = 166,
            .y = 63,
        },
        .tgt = (SDL_Rect) {
            .h = h,
            .w = w,
            .x = x,
            .y = y,
        },
    };
    return ctx;
}

ctx_t * o_flash_update (ctx_t * ctx) {
    ctx->flash.show = true;
    return ctx;
}
