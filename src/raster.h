#ifndef _RASTER_H
#define _RASTER_H

#include <SDL2/SDL.h>
#include "la.h"
#include "result.h"
#include "colour.h"

typedef struct rt_ctx_s {
    u32 fbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
    f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
} rt_ctx_t;

void rt_ctx_clear(rt_ctx_t *ctx);
result_t rt_ctx_init(rt_ctx_t **ctx_pp);
void rt_ctx_destroy(rt_ctx_t *ctx);

void rt_pixel(rt_ctx_t *ctx, i32 x, i32 y, f32 z, col_t col);
void rt_pixel_vec(rt_ctx_t *ctx, v3 v, col_t col);

void rt_line(rt_ctx_t *ctx, v3 p1, v3 p2, col_t col);

void rt_tri3c(rt_ctx_t *ctx, v3 a, v3 b, v3 c,
              col_t c1, col_t c2, col_t c3);
void rt_tri(rt_ctx_t *ctx, v3 a, v3 b, v3 c, col_t col);

#endif //_RASTER_H
