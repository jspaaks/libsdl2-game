#include "s_playing.h"
#include "types.h"
#include "constants.h"
#include "fsm.h"
#include "o_background.h"
#include "o_moon.h"
#include "o_ground.h"
#include "o_turret.h"
#include "o_barrel.h"
#include "o_flash.h"
#include "o_balloons.h"
#include "o_bullets.h"
#include "o_collisions.h"
#include "o_legend.h"
#include <SDL_render.h>
#include <SDL_rect.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>

void s_playing_draw (ctx_t * ctx) {
    o_background_draw(ctx);
    o_moon_draw(ctx);
    o_turret_draw(ctx);
    o_barrel_draw(ctx);
    o_flash_draw(ctx);
    o_legend_draw(ctx);
    o_balloons_draw(ctx);
    o_bullets_draw(ctx);
    o_collisions_draw(ctx);
    o_ground_draw(ctx);
}

ctx_t * s_playing_update (ctx_t * ctx, struct state ** state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                fprintf(stdout, "pausing\n");
                *state = fsm_set_state(PAUSED);
            }
        }
    }
    ctx = o_legend_update(ctx);
    ctx = o_turret_update(ctx);
    ctx = o_barrel_update(ctx);
    ctx = o_flash_update(ctx);
    ctx = o_balloons_update(ctx);
    ctx = o_bullets_update(ctx);
    ctx = o_collisions_update(ctx);
    SDL_RenderPresent(ctx->renderer);
    return ctx;
}
