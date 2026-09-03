#ifndef _RASTER_H
#define _RASTER_H

#include <SDL2/SDL.h>
#include "la.h"
#include "result.h"
#include "colour.h"

typedef struct raster_ctx_s {
    u32 fbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
    f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
} raster_ctx_t;

void raster_ctx_clear(raster_ctx_t *ctx);
result_t raster_ctx_init(raster_ctx_t **ctx_pp);
void raster_ctx_destroy(raster_ctx_t *ctx);

void raster_pixel(raster_ctx_t *ctx, i32 x, i32 y, f32 z, col_t col);
void raster_pixel_vec(raster_ctx_t *ctx, v3 v, col_t col);

void raster_line(raster_ctx_t *ctx, v3 p1, v3 p2, col_t col);

void raster_tri3c(raster_ctx_t *ctx, v3 a, v3 b, v3 c,
                  col_t c1, col_t c2, col_t c3);
void raster_tri(raster_ctx_t *ctx, v3 a, v3 b, v3 c, col_t col);

#endif //_RASTER_H
