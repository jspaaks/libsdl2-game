#include <assert.h>
#include "SDL_render.h"
#include "SDL_rect.h"
#include "types.h"
#include "o_turret.h"
#include "o_scene.h"

void o_turret_draw (SDL_Renderer * renderer, scene_t scene, SDL_Texture * spritesheet, turret_t turret) {
    SDL_Rect tgt = sim2tgt(scene, turret.sim);
    SDL_RenderCopy(renderer, spritesheet, &turret.src, &tgt);
}

void o_turret_init (scene_t scene, ground_t ground, turret_t * turret) {
    assert(ground.sim.w != 0 && "ground needs to be initialized before turret");
    float h = 47;
    SDL_FRect sim = {
        .h = h,
        .w = 69,
        .x = 180,
        .y = scene.sim.h - ground.sim.h - h,
    };
    *turret = (turret_t){
        .sim = sim,
        .src = (SDL_Rect) {
            .h = 47,
            .w = 69,
            .x = 4,
            .y = 1,
        },
    };
}
