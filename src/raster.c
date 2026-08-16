#include "raster.h"
#include "aabb.h"
#include "transform.h"

void rt_ctx_clear(rt_ctx_t *ctx)
{
    memset(ctx->fbuffer, 0, sizeof(ctx->fbuffer));

    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        ctx->zbuffer[i] = FLT_MAX;
    }
}

result_t rt_ctx_init(rt_ctx_t **ctx_pp)
{
    rt_ctx_t *ctx = (rt_ctx_t*)malloc(sizeof(*ctx));
    HANDLE_ERROR_RET(!ctx, RESULT_ERROR_ALLOC,
                     "rt ctx init alloc");
    rt_ctx_clear(ctx);
    *ctx_pp = ctx;

    return RESULT_OK;
}

void rt_ctx_destroy(rt_ctx_t *ctx)
{
    free(ctx);
}

void rt_put_pixel(rt_ctx_t *ctx, i32 x, i32 y, f32 z, colour_t col)
{
    if (!IN_BOUNDS(x, y)) {
        DEBUG(3, "pixel out of bounds: {%d, %d}", x, y);
        return;
    }
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (ctx->zbuffer[idx]> z) {
        ctx->zbuffer[idx] = z;
        ctx->fbuffer[idx] = COL_TO_HEX(col);
    }
}

void rt_put_pixel_vec(rt_ctx_t *ctx, v3 v, colour_t col)
{
    i32 x = (i32)v.x;
    i32 y = (i32)v.y;

    rt_put_pixel(ctx, x, y, v.z, col);
}

/*
 * rt_put_line()
 * this function works as follows: 
 * youve got two points, p0, p1, depending on the slope of the line between
 * these points, we do one of these two things:
 *              (1) for every x check if we change y by 1
 *                              OR
 *              (2) for every y check if we change x by 1
 
 * 1 is the default, but we check `is_steep` in which case we swap the x and y to
 * enable us to do 2.
  
 * The second SWAP makes sure that we are always drawing left->right
 
 * `diff` keeps track of how much we have deviated from our original axis,
 * which lets us know if we should change the second axis by 1.
 
 * The 'first' axis is the one being controlled in the for loop, and the `second`
 * axis is the one that changes by one when the diff deviation surpasses the
 * delta of the second axis, then we subtract 2*(delta(2)-delta(1))
 
 * when we're putting pixels, we then must check if is_steep, in  which case we
 * must put the pixels in reverse order essentially undo the swap above.
 */
void rt_put_line(rt_ctx_t *ctx, v3 p0, v3 p1, colour_t col)
{
    v3 p0s, p1s;
    if (!trans_world_to_screen(p0, &p0s)) return;
    if (!trans_world_to_screen(p1, &p1s)) return;
    DEBUG(3, "p0s = %f %f | p1f = %f %f",
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
        DEBUG(3, "DIFF: %d", diff);

        if (is_steep) rt_put_pixel(ctx, y, x, 0, col);
        else          rt_put_pixel(ctx, x, y, 0, col);

        if (diff >= 0) {
            y += yi;
            diff += (2 * (dy - dx));
        } else {
            diff += (2 * dy);
        }
    }
}

/*
 * rt_put_tri()
 * We translate each point to screen space, then calculate the bounding box of
 * the triangle. Then for each pixel P inside this bounding box, we check the 
 * barycentric coordinates of P, and if each x,y,z of that coord is positive, 
 * then P is inside the triangle, and therefore we can put a pixel at P.
 */
void rt_put_tri(rt_ctx_t *ctx,
                v3 a, v3 b, v3 c, colour_t col)
{
    v3 as, bs, cs;
    if (!trans_world_to_screen(a, &as) || 
        !trans_world_to_screen(b, &bs) ||
        !trans_world_to_screen(c, &cs)) {
        DEBUG(3, "returning early");
        return;
    } 

    aabb_t tri_aabb = aabb_get(as, bs, cs);
    DEBUG(2, "AABB: min %f %f -- max %f %f",
          tri_aabb.min.x, tri_aabb.min.y,
          tri_aabb.max.x, tri_aabb.max.y);
    v2i i;
    for (i.x = tri_aabb.min.x; i.x < tri_aabb.max.x; i.x++) {
        for (i.y = tri_aabb.min.y; i.y < tri_aabb.max.y; i.y++) {
            v3 bary = barycentric(V3_TO_V2i(as), 
                                  V3_TO_V2i(bs),
                                  V3_TO_V2i(cs),
                                  i);
            DEBUG(0, "BARY (%d, %d): %f %f %f",
                  i.x, i.y, bary.x, bary.y, bary.z);

            if (!v3_contains_neg(bary)) {
                rt_put_pixel(ctx, i.x, i.y, 0,
                             BARY_COL(bary, col));
            }
        }
    }
}

