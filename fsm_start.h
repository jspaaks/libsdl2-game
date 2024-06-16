#ifndef FSM_START_H
#define FSM_START_H
#include "SDL_render.h"
#include "SDL_video.h"
#include "types.h"

void fsm_start_draw (ctx_t, drawing_t, drawables_t, counters_t);
void fsm_start_update (timing_t, chunks_t, counters_t *, ctx_t *, drawing_t *, drawables_t *, gamestate_t **);

#endif
