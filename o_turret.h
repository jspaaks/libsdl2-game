#ifndef O_TURRET_H
#define O_TURRET_H
#include "SDL_render.h"
#include "types.h"

void o_turret_draw (SDL_Renderer *, scene_t, SDL_Texture *, turret_t);
void o_turret_init (scene_t, ground_t, turret_t *);

#endif
