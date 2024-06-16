#include <stdio.h>
#include <assert.h>
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "scene.h"
#include "types.h"
#include "o_flash.h"

void o_flash_draw (SDL_Renderer * renderer, SDL_Texture * spritesheet, scene_t scene, barrel_t barrel, flash_t flash) {
    bool show = flash.countdown_remaining > 0 && flash.had_bullets;
    if (show) {
        SDL_Rect tgt = sim2tgt(scene, flash.sim);
        SDL_Point pivot_offset = (SDL_Point) {
            .x = (int) (flash.sim2.pivot_offset.x * scene.scale),
            .y = (int) (flash.sim2.pivot_offset.y * scene.scale),
        };
        SDL_RenderCopyEx(renderer, spritesheet, &flash.src,
                                                &tgt,
                                                barrel.sim2.angle,
                                                &pivot_offset,
                                                SDL_FLIP_NONE);
    }
}

flash_t o_flash_init (barrel_t barrel) {
    assert(barrel.sim.x != 0 && "barrel needs to be initialized before flash");
    float h = 21;
    float w = 30;
    float loffset = (barrel.sim.w + 2);
    SDL_Rect src = {
        .h = h,
        .w = w,
        .x = 166,
        .y = 63,
    };
    SDL_FRect sim = {
        .h = h,
        .w = w,
        .x = barrel.sim2.pivot.x + loffset,
        .y = barrel.sim2.pivot.y - (h - 1) / 2,
    };
    return (flash_t) {
        .countdown_duration = 30,
        .countdown_remaining = 0,
        .sim = sim,
        .sim2 = {
            .pivot_offset = (SDL_FPoint) {
                .x = -1 * loffset,
                .y = (h - 1) / 2,
            },
        },
        .src = src,
    };
}

void o_flash_update (timing_t timing, flash_t * flash) {
    if (flash->countdown_remaining > 0) {
        flash->countdown_remaining -= timing.dt.frame;
    }
}
