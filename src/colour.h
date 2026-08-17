#ifndef _COLOUR_H
#define _COLOUR_H

#include "la.h"

typedef struct colour_s {
    u8 r, g, b, a;
} colour_t;

#define COL_TO_HEX(col) (u32)((col).a<<24)|\
                        (u32)((col).r<<16)|\
                        (u32)((col).g<<8)| \
                        (u32)(col).b

static inline colour_t
colerp(colour_t col, f32 x, f32 y, f32 z)
{
    return (colour_t){
        col.r * x,
        col.g * y,
        col.b * z,
        col.a
    };
}

static inline colour_t
colerpv(colour_t col, v3 v)
{
    return colerp(col, v.x, v.y, v.z);
}


#endif //_COLOUR_H
