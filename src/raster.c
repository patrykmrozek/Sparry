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

raster_context_t *raster_context_init(void)
{
    raster_context_t *raster_ctx = 
        (raster_context_t*)malloc(sizeof(*raster_ctx));
    raster_context_clear(raster_ctx);

    return raster_ctx;
}

void raster_context_destroy(raster_context_t *raster_ctx)
{
    free(raster_ctx);
}

void raster_put_pixel(raster_context_t *raster_ctx, i32 x, i32 y, i32 z, u32 c)
{
    if (!IN_BOUNDS(x, y)) return;
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> z) {
        raster_ctx->zbuffer[idx] = z;
        raster_ctx->framebuffer[idx] = c;
    }
}

void raster_put_pixel_vec(raster_context_t *raster_ctx, v3 v, u32 c)
{
    if (!IN_BOUNDS(v.x, v.y)) return;
    u32 idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!raster_ctx->zbuffer[idx] || raster_ctx->zbuffer[idx]> v.z) {
        raster_ctx->zbuffer[idx] = v.z;
        raster_ctx->framebuffer[idx] = c;
    }
}

void raster_put_line(raster_context_t *raster_ctx, v3 p1, v3 p2, u32 color)
{
    v3 p1_screen, p2_screen;
    if (!world_to_screen(p1, &p1_screen)) return;
    if (!world_to_screen(p2, &p2_screen)) return;
    printf("p1s = %f %f | p2f = %f %f\n", 
            p1_screen.x, p1_screen.y, p2_screen.x, p2_screen.y);

    i32 err = 0;
    i32 dx = p2_screen.x - p1_screen.x;
    i32 dy = p2_screen.y - p1_screen.y;
    printf("dx: %i, dy: %i\n", dx, dy);
    int y = p1_screen.y;
    
    for (int x = p1_screen.x; x < p2_screen.x; x++) {
        printf("x: %i, y: %i\n", x, y);
        printf("\t%d\n", IN_BOUNDS(x, y));
        if (IN_BOUNDS(x, y)) {
            raster_put_pixel(raster_ctx, x, y, 0, color);
        }
        err += dy;
        if (err >= dx) {
            y++;
            err -= dx;
        }
    }
}

#if 0
void raster_put_line(raster_context_t *raster_ctx, v3 p1, v3 p2, u32 color)
{
    u32 err = 0;
    u32 dx = p2.x - p1.x;
    u32 dy = p2.y - p1.y;
    printf("dx: %i, dy: %i\n", dx, dy);
    int y = dy;
    
    for (int x = p1.x; x < p2.x; x++) {
        printf("x: %i, y: %i\n", x, y);
        v3 p_screen;
        if (!world_to_screen((v3){x, y, 0}, &p_screen)) return;
        raster_put_pixel(raster_ctx, x, y, 0, color);
        err += dy;
        if (err >= dx) {
            y++;
            err -= dx;
        }
    }
}
#endif


