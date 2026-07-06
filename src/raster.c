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

//this function works as follows: 
//youve got two points, p0, p1,
//depending on the slope of the
//line between these points, we 
//do one of these two things:
//(1) for every x check if we change y by 1
//              OR
//(2) for every y check if we change x by 1

//1 is the default, but we check `is_steep`
//in which case we swap the x and y to
//enable us to do 2.
 
//The second SWAP makes sure that we 
//are always drawing left->right

//`diff` keeps track of how much we
//have deviated from our original
//axis, which lets us know if we 
//should change the second axis by 1.

//The 'first' axis is the one being
//controlled in the for loop, and
//the `second` axis is the one 
//that changes by one when the 
//diff deviation surpasses the
//delta of the second axis, then
//we subtract 2*(delta(2)-delta(1))

//when we're putting pixels, we
//then must check if is_steep, in 
//which case we must put the pixels
//in reverse order essentially undo
//the swap above.
void raster_put_line(raster_context_t *raster_ctx,
                     v3 p0, v3 p1, u32 c)
{
    v3 p0s, p1s;
    if (!world_to_screen(p0, &p0s)) return;
    if (!world_to_screen(p1, &p1s)) return;
    LOG(LOG_LEVEL_DEBUG,
        "p0s = %f %f | p1f = %f %f", 
        p0s.x, p0s.y, p1s.x, p1s.y);

    i32 x0 = (i32)p0s.x;
    i32 y0 = (i32)p0s.y;
    i32 x1 = (i32)p1s.x;
    i32 y1 = (i32)p1s.y;

    bool is_steep = abs(y1 - y0) > abs(x1 - x0); 

    if (is_steep) {
        SWAP(x0, y0);
        SWAP(x1, y1);
    }

    if (x0 > x1) {
        SWAP(x0, x1);
        SWAP(y0, y1);
    }

    i32 dx = x1 - x0;
    i32 dy = abs(y1 - y0);
    i32 yi = (y1 - y0 < 0) ? -1 : 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    i32 diff = (2 * dy) - dx;
    i32 y = y0;

    for (i32 x = x0; x < x1; x++) {
        LOG(LOG_LEVEL_DEBUG,
             "DIFF: %d",
             diff);
        if (is_steep) {
            if (IN_BOUNDS(y, x)) {
                raster_put_pixel(raster_ctx, y, x, 0, c);
            }
        } else {
            if (IN_BOUNDS(x, y)) {
                raster_put_pixel(raster_ctx, x, y, 0, c);
            }
        }
        if (diff >= 0) {
            y += yi;
            diff += (2 * (dy - dx));
        } else {
            diff += (2 * dy);
        }
    }
}

