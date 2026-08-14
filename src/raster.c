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
        ERROR("raster context init allocation failed");
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
        DEBUG("pixel out of bounds: {%d, %d}", x, y);
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

    raster_put_pixel(raster_ctx, x, y, v.z, c);
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
    DEBUG("p0s = %f %f | p1f = %f %f",
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
    
    i32 diff = (2 * dy) - dx;
    i32 y = y0;

    for (i32 x = x0; x < x1; x++) {
        DEBUG("DIFF: %d", diff);

        if (is_steep) raster_put_pixel(raster_ctx, y, x, 0, c);
        else          raster_put_pixel(raster_ctx, x, y, 0, c);

        if (diff >= 0) {
            y += yi;
            diff += (2 * (dy - dx));
        } else {
            diff += (2 * dy);
        }
    }
}

#define MIN(a, b) (a < b) ? a : b
#define MAX(a, b) (a > b) ? a : b

typedef struct aabb_s {
    v3 min, max;
} aabb_t;

aabb_t get_aabb(v3 p1, v3 p2, v3 p3)
{
    aabb_t out;
    out.min = (v3){
        MIN(MIN(p1.x, p2.x), p3.x),
        MIN(MIN(p1.y, p2.y), p3.y),
        MIN(MIN(p1.z, p2.z), p3.z),
    };
    out.max = (v3){
        MAX(MAX(p1.x, p2.x), p3.x),
        MAX(MAX(p1.y, p2.y), p3.y),
        MAX(MAX(p1.z, p2.z), p3.z),
    };

    return out;
}

#define CONTAINS_NEG(v) (v.x < 0 || v.y < 0 || v.z < 0)

void raster_put_triangle(raster_context_t *raster_ctx,
                         v3 p1, v3 p2, v3 p3, u32 c)
{
    if (p1.y > p2.y) SWAP(p1, p2);
    if (p1.y > p3.y) SWAP(p1, p3);
    if (p2.y > p3.y) SWAP(p2, p3);

    v3 p1s, p2s, p3s;
    if (!world_to_screen(p1, &p1s) || 
        !world_to_screen(p2, &p2s) ||
        !world_to_screen(p3, &p3s)) {
        TRACE("returning early");
        return;
    } 

    aabb_t tri_aabb = get_aabb(p1s, p2s, p3s);
    TRACE("AABB: min %f %f -- max %f %f",
          tri_aabb.min.x, tri_aabb.min.y,
          tri_aabb.max.x, tri_aabb.max.y);
    v3 i;
    for (i.x = tri_aabb.min.x; i.x < tri_aabb.max.x; i.x++) {
        for (i.y = tri_aabb.min.y; i.y < tri_aabb.max.y; i.y++) {
            v3 bary = barycentric(p1s, p2s, p3s, i);
            TRACE("BARY: %f %f %f", bary.x, bary.y, bary.z);
            if (!CONTAINS_NEG(bary)) {
                raster_put_pixel(raster_ctx, i.x, i.y, 0, c);
            }
        }
    }
}

