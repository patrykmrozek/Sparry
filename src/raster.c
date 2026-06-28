#include "raster.h"

void raster_context_clear(raster_context_t *raster_ctx)
{
    memset(raster_ctx->framebuffer, 0, sizeof(raster_ctx->framebuffer));

    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        raster_ctx->zbuffer[i] = FLT_MAX;
    }
}

raster_context_t *raster_context_init(void)
{
    raster_context_t *raster_ctx = (raster_context_t*)malloc(sizeof(*raster_ctx));
    raster_context_clear(raster_ctx);

    return raster_ctx;
}

void put_pixel(raster_context_t *raster_ctx, i32 x, i32 y, i32 z, u32 c)
{
    u32 idx;

    idx = (y * SCREEN_WIDTH) + x;
        if (idx < 0 || idx >= SCREEN_HEIGHT*SCREEN_WIDTH) {
            return;
        }
        if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> z) {
            raster_ctx->zbuffer[idx] = z;
            raster_ctx->framebuffer[idx] = c;
    }
}

void put_pixel_vec(raster_context_t *raster_ctx, v3 v, u32 c)
{
    u32 idx;

    v.x = (i32)v.x;
    v.y = (i32)v.y;
    if (v.x < 0 || v.x >= SCREEN_WIDTH || v.y < 0 || v.y >= SCREEN_HEIGHT) return;

    idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> v.z) {
        raster_ctx->zbuffer[idx] = v.z;
        raster_ctx->framebuffer[idx] = c;
    }
}

