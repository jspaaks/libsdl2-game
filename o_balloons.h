#ifndef BALLOONS_H
#define BALLOONS_H
#include "types.h"

void o_balloons_draw (ctx_t *);
ctx_t * o_balloons_deinit (ctx_t *);
ctx_t * o_balloons_init (ctx_t *);
ctx_t * o_balloons_update (ctx_t *);
ctx_t * o_balloons_update_remove (ctx_t *);

#endif
