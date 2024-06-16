#include "ctx.h"
#include "types.h"
#include "chunks.h"
#include "keystate.h"
#include "levels.h"


ctx_t ctx_init (void) {
    ctx_t ctx;
    keystate_init(&ctx);
    chunks_init(&ctx);
    levels_init(&ctx);
    ctx.isfullscreen = false;
    ctx.resized = true;
    return ctx;
}
