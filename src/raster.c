#include "raster.h"
#include "common.h"
#include "transform.h"

void raster_context_clear(raster_context_t *raster_ctx)
{
    memset(raster_ctx->framebuffer, 0, sizeof(raster_ctx->framebuffer));

    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        raster_ctx->zbuffer[i] = FLT_MAX;
    }
}

result_t raster_context_init(raster_context_t **out_raster_ctx)
{
    raster_context_t *raster_ctx = 
        (raster_context_t*)malloc(sizeof(*raster_ctx));
    if (!raster_ctx) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "raster context init allocation failed");
        return RESULT_ERROR_ALLOC;
    }
    raster_context_clear(raster_ctx);
    *out_raster_ctx = raster_ctx;

    return RESULT_OK;
}

void raster_context_destroy(raster_context_t *raster_ctx)
{
    free(raster_ctx);
}

void raster_put_pixel(raster_context_t *raster_ctx, 
                      i32 x, i32 y, f32 z, u32 c)
{
    if (!IN_BOUNDS(x, y)) {
        LOG(LOG_LEVEL_DEBUG,
            "pixel out of bounds: {%d, %d}",
            x, y);
        return;
    }
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> z) {
        raster_ctx->zbuffer[idx] = z;
        raster_ctx->framebuffer[idx] = c;
    }
}

void raster_put_pixel_vec(raster_context_t *raster_ctx, v3 v, u32 c)
{
    i32 x = (i32)v.x;
    i32 y = (i32)v.y;

    if (!IN_BOUNDS(x, y)) {
        LOG(LOG_LEVEL_DEBUG,
            "pixel out of bounds: {%d, %d}",
            x, y);
        return;
    }
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> v.z) {
        raster_ctx->zbuffer[idx] = v.z;
        raster_ctx->framebuffer[idx] = c;
    }
}

//slope <= 1
static inline void _raster_put_line_low(raster_context_t *raster_ctx,
                                        i32 x0, i32 y0,
                                        i32 x1, i32 y1,
                                        u32 c)
{
    i32 dx = x1 - x0;
    i32 dy = y1 - y0;
    i32 yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    i32 diff = (2 * dy) - dx;
    i32 y = y0;

    for (i32 x = x0; x < x1; x++) {
        if (IN_BOUNDS(x, y)) {
            raster_put_pixel(raster_ctx, x, y, 0, c);
        }
        if (diff >= 0) {
            y += yi;
            diff += (2 * (dy - dx));
        } else {
            diff += (2 * dy);
        }
    }
}

//when slope >= 1
static inline void _raster_put_line_high(raster_context_t *raster_ctx,
                                         i32 x0, i32 y0,
                                         i32 x1, i32 y1,
                                         u32 c)
{
    i32 dx = x1 - x0;
    i32 dy = y1 - y0;
    i32 xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    i32 diff = (2 * dx) - dy;
    i32 x = x0;

    for (i32 y = y0; y < y1; y++) {
        if (IN_BOUNDS(x, y)) {
            raster_put_pixel(raster_ctx, x, y, 0, c);
        }
        if (diff >= 0) {
            x += xi;
            diff += (2 * (dx - dy));
        } else {
            diff += (2 * dx);
        }
    }
}

void raster_put_line(raster_context_t *raster_ctx,
                     v3 p0, v3 p1, u32 color)
{
    v3 p0s, p1s;
    if (!world_to_screen(p0, &p0s)) return;
    if (!world_to_screen(p1, &p1s)) return;
    LOG(LOG_LEVEL_DEBUG, "p0s = %f %f | p1f = %f %f", 
            p0s.x, p0s.y, p1s.x, p1s.y);
    i32 p0sx = (i32)p0s.x;
    i32 p0sy = (i32)p0s.y;
    i32 p1sx = (i32)p1s.x;
    i32 p1sy = (i32)p1s.y;

    if (abs(p1sy - p0sy) < abs(p1sx - p0sx)) {
        if (p0sx > p1sx) {
            _raster_put_line_low(raster_ctx,
                                 p1sx, p1sy,
                                 p0sx, p0sy,
                                 color);
        } else {
            _raster_put_line_low(raster_ctx,
                                 p0sx, p0sy,
                                 p1sx, p1sy,
                                 color);
        }
    } else {
        if (p0sy > p1sy) {
            _raster_put_line_high(raster_ctx,
                                  p1sx, p1sy,
                                  p0sx, p0sy,
                                  color);
        } else {
            _raster_put_line_high(raster_ctx,
                                  p0sx, p0sy,
                                  p1sx, p1sy,
                                  color);
        }
    }
}


