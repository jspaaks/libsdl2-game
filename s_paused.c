#include "s_paused.h"
#include "o_background.h"
#include "types.h"
#include "fsm.h"
#include <SDL_events.h>
#include <SDL_keycode.h>

void paused_draw (ctx_t * ctx) {
    background_draw(ctx);
}

void paused_update (struct state ** game, SDL_Event * event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            fprintf(stdout, "Playing.\n");
            *game = fsm_set_state(PLAYING);
        }
        if (event->key.keysym.sym == SDLK_q) {
            fprintf(stdout, "Quitting.\n");
            exit(EXIT_SUCCESS);
        }
    }
}
