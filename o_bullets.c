#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "SDL_mixer.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_scancode.h"
#include "SDL_log.h"
#include "SDL_error.h"
#include "o_bullets.h"
#include "o_scene.h"

static void o_bullets_update_pos (ctx_t *, bullet_t **);
static void o_bullets_update_remove (ctx_t *, bullet_t **);
static void o_bullets_update_spawn (ctx_t *, barrel_t, bullet_t **);
static void o_bullets_update_test_exited (scene_t, ground_t, ctx_t *,  bullet_t **);

void o_bullets_deinit (bullet_t ** bullets) {
    bullet_t * b = *bullets;
    while (b != NULL) {
        bullet_t * tmp = b;
        b = b->next;
        free(tmp);
    }
    *bullets = NULL;
}

void o_bullets_draw (SDL_Renderer * renderer, scene_t scene, SDL_Texture * spritesheet, bullet_t * bullets) {
    bullet_t * bu = bullets;
    while (bu != NULL) {
        SDL_Rect tgt = sim2tgt(scene, bu->sim);
        SDL_RenderCopy(renderer, spritesheet, bu->src, &tgt);
        bu = bu->next;
    }
}

void o_bullets_init (level_t * level, ground_t ground, bullet_t ** bullets, nbullets_t * nbullets) {
    assert(level != NULL && "levels needs to be initialized before bullets");
    assert(ground.sim.w != 0 && "ground needs to be initialized before bullets");
    *bullets = NULL;
    nbullets->prespawn = level->nbullets.prespawn;
    nbullets->airborne = 0;
}

void o_bullets_update (scene_t scene, ground_t ground, ctx_t * ctx, barrel_t barrel, bullet_t ** bullets) {
    // mark bullets that are out of frame
    o_bullets_update_test_exited(scene, ground, ctx, bullets);

    // if bullet is marked for deletion, delete it from the list
    o_bullets_update_remove(ctx, bullets);

    // update position
    o_bullets_update_pos(ctx, bullets);

    // if SPACE down, add a bullet to the list
    o_bullets_update_spawn(ctx, barrel, bullets);
}

static void o_bullets_update_spawn (ctx_t * ctx, barrel_t barrel, bullet_t ** bullets) {
    static const float PI = 3.14159265358979323846f;
    Uint64 timeout = 150;
    static SDL_Rect src_bullet = { .x = 188, .y = 38, .w = 5, .h = 5 };
    bool has_bullets = ctx->nbullets.prespawn > 0;
    bool key_pressed = ctx->keys[SDL_SCANCODE_SPACE];
    bool cooled_off = SDL_GetTicks64() > ctx->tspawn_latestbullet + timeout;

    if (key_pressed && cooled_off) {
        if (has_bullets) {
            bullet_t * b = malloc(1 * sizeof(bullet_t));
            if (b == NULL) {
                SDL_LogError(SDL_ENOMEM, "Something went wrong allocating memory for new bullet.\n");
                exit(EXIT_FAILURE);
            }
            float a = PI * barrel.sim2.angle / 180;
            float x = barrel.sim2.pivot.x + cos(a) * (barrel.sim2.length + 20) - (src_bullet.w - 1) / 2;
            float y = barrel.sim2.pivot.y + sin(a) * (barrel.sim2.length + 20) - (src_bullet.h - 1) / 2;

            float speed = 380;
            *b = (bullet_t) {
                .next = *bullets,
                .sim = {
                    .x = x,
                    .y = y,
                    .w = 5,
                    .h = 5,
                },
                .sim2 = {
                    .u = cos(a) * speed,
                    .v = sin(a) * speed,
                },
                .src = &src_bullet,
                .state = ALIVE,
            };
            *bullets = b;
            ctx->nbullets.prespawn--;
            ctx->nbullets.airborne++;
            ctx->tspawn_latestbullet = SDL_GetTicks64();
            Mix_PlayChannel(-1, ctx->chunks.shoot, 0);
        } else {
            Mix_PlayChannel(-1, ctx->chunks.empty, 0);
            ctx->tspawn_latestbullet = SDL_GetTicks64();
        }
    }
}

static void o_bullets_update_pos (ctx_t * ctx, bullet_t ** bullets) {
    const float gravity = 70; // pixels per second per second
    bullet_t * b = *bullets;
    while (b != NULL) {
        b->sim2.v += gravity * ctx->dt.frame;
        b->sim.x += b->sim2.u * ctx->dt.frame;
        b->sim.y += b->sim2.v * ctx->dt.frame;
        b = b->next;
    }
}

void o_bullets_update_remove (ctx_t * ctx, bullet_t ** bullets) {
    bullet_t * this = *bullets;
    bullet_t * prev = NULL;
    bool isfirst = false;
    bool doremove = false;
    while (this != NULL) {
        isfirst = prev == NULL;
        doremove = this->state != ALIVE;
        switch (isfirst << 1 | doremove ) {
            case 0: {
                // not first, not remove
                prev = this;
                this = this->next;
                break;
            }
            case 1: {
                // not first, remove
                bullet_t * tmp = this;
                prev->next = this->next;
                if (this->state == HIT || this->state == EXITED) {
                    ctx->nbullets.airborne--;
                }
                this = this->next;
                free(tmp);
                break;
            }
            case 2: {
                // first, not remove
                prev = this;
                this = this->next;
                break;
            }
            case 3: {
                // first, remove
                bullet_t * tmp = this;
                if (this->state == HIT || this->state == EXITED) {
                    ctx->nbullets.airborne--;
                }
                *bullets = this->next;
                this = this->next;
                free(tmp);
                break;
            }
            default: {
                SDL_LogError(SDL_UNSUPPORTED, "Something went wrong in removing a bullet from the list.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void o_bullets_update_test_exited (scene_t scene, ground_t ground, ctx_t * ctx,  bullet_t ** bullets) {
    bullet_t * this = *bullets;
    bool exited;
    while (this != NULL) {
        exited = this->sim.y < 0 - this->sim.h  ||
                 this->sim.x > scene.sim.w ||
                 this->sim.x < 0 - this->sim.w  ||
                 this->sim.y > scene.sim.h - ground.sim.h;
        if (exited) {
            this->state = EXITED;
        }
        this = this->next;
    }
}
