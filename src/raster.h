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

void rt_put_pixel(rt_ctx_t *ctx, i32 x, i32 y, f32 z, colour_t col);
void rt_put_pixel_vec(rt_ctx_t *ctx, v3 v, colour_t col);

void rt_put_line(rt_ctx_t *ctx, v3 p1, v3 p2, colour_t col);

void rt_put_tri(rt_ctx_t *ctx, v3 p1, v3 p2, v3 p3, colour_t col);

#endif //_RASTER_H
