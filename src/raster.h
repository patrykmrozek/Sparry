#ifndef _RASTER_H
#define _RASTER_H

#include <SDL2/SDL.h>
#include "la.h"

typedef struct raster_context_s {
    u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
    f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
} raster_context_t;

void raster_context_clear(raster_context_t *raster_ctx);
raster_context_t *raster_context_init(void);
void raster_context_destroy(raster_context_t *raster_ctx);

void raster_put_pixel(raster_context_t *raster_ctx, i32 x, i32 y, i32 z, u32 c);
void raster_put_pixel_vec(raster_context_t *raster_ctx, v3 v, u32 c);

void raster_put_line(raster_context_t *raster_ctx, v3 p1, v3 p2, u32 color);

#endif //_RASTER_H
