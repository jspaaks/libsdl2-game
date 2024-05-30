#include "constants.h"
#include "context.h"
#include "fsm.h"
#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL_timer.h>
#include "o_balloons.h"

void deinit (ctx_t *);
bool init (ctx_t *);

void deinit (ctx_t * ctx) {
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
    free(ctx->balloons);
    ctx->window = NULL;
    ctx->renderer = NULL;
    ctx->spritesheet = NULL;
    ctx->balloons = NULL;
}

bool init (ctx_t * ctx) {
    int flags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (SDL_Init(flags) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    ctx->window = SDL_CreateWindow("Midnight Balloon Murder", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                   SDL_WINDOW_RESIZABLE);
    if (ctx->window == NULL) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);
    SDL_Surface * image = SDL_LoadBMP("img/sprites.bmp");
    if (image == NULL) {
        fprintf(stdout, "Something went wrong loading spritesheet.\n");
        return false;
    }
    ctx->spritesheet = SDL_CreateTextureFromSurface(ctx->renderer, image);
    ctx->keys = SDL_GetKeyboardState(NULL);
    ctx->dt = 0.0000000000001;
    ctx->nballoons = 10;
    ctx->balloons = balloons_init(ctx->nballoons);
    if (ctx->balloons == NULL) {
        fprintf(stderr, "Something went wrong allocating memory for the balloons.\n");
        return false;
    }
    return true;
}

int main (void) {
    ctx_t ctx;
    if (!init(&ctx)) {
        exit(EXIT_FAILURE);
    }

    struct state * state = fsm_set_state (PLAYING);
    struct state * frame = state;
    Uint64 tstart;

    Uint64 timeout = SDL_GetTicks64() + 15000;
    while (SDL_GetTicks64() < timeout) {
        tstart = SDL_GetTicks64();
        frame = state;  // so .update() and .draw() are of the same state
        frame->update(&ctx, &state);
        frame->draw(&ctx);
        ctx.dt = ((double) (SDL_GetTicks64() - tstart)) / 1000;
    }

    deinit(&ctx);
    return EXIT_SUCCESS;
}
