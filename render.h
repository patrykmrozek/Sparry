#ifndef _RENDERER_H
#define _RENDERER_H

#include "la.h"

static u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static inline void put_pixel(i32 x, i32 y, i32 z, u32 c)
{
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (idx < 0 || idx >= SCREEN_HEIGHT*SCREEN_WIDTH) {
        return;
    }
    if (!zbuffer[idx] || zbuffer[idx]> z) {
        zbuffer[idx] = z;
        framebuffer[idx] = c;
    }
}
static inline void put_pixel_vec(v3 v, u32 c)
{
    v.x = (i32)v.x;
    v.y = (i32)v.y;
    if (v.x < 0 || v.x >= SCREEN_WIDTH || v.y < 0 || v.y >= SCREEN_HEIGHT) return;

    u32 idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!zbuffer[idx] || zbuffer[idx]> v.z) {
        zbuffer[idx] = v.z;
        framebuffer[idx] = c;
    }
}

#endif
