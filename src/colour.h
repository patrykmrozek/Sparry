#ifndef _COLOUR_H
#define _COLOUR_H

#include "la.h"

typedef struct col_s {
    u8 r, g, b, a;
} col_t;

#define COL_WHITE  (col_t){255, 255, 255, 255}
#define COL_BLACK  (col_t){0,   0,   0,   0  }
#define COL_RED    (col_t){255, 0,   0,   255}
#define COL_GREEN  (col_t){0,   255, 0,   255}
#define COL_BLUE   (col_t){0,   0,   255, 255}
#define COL_YELLOW (col_t){255, 255, 0,   255}
#define COL_PURPLE (col_t){255, 0,   255, 255}
#define COL_LBLUE  (col_t){0,   255, 255, 255}
#define COL_ORANGE (col_t){255, 200, 0,   255}
#define COL_PINK   (col_t){255, 0,   150, 255}
#define COL_LGREY  (col_t){150, 150, 150, 255}
#define COL_DGREY  (col_t){50, 50, 50, 255}

#define COL_TO_HEX(col) (u32)((col).a<<24)|\
                        (u32)((col).r<<16)|\
                        (u32)((col).g<<8)| \
                        (u32)(col).b

static inline col_t
colerp(col_t col, f32 x, f32 y, f32 z)
{
    return (col_t){
        col.r * x,
        col.g * y,
        col.b * z,
        col.a
    };
}

static inline col_t
colerpv(col_t col, v3 v)
{
    return colerp(col, v.x, v.y, v.z);
}

static inline
col_t col_blend3(col_t c1, col_t c2, col_t c3, v3 p)
{
    col_t out;
    out.r = (u8)(c1.r*p.x + c2.r*p.y + c3.r*p.z);
    out.g = (u8)(c1.g*p.x + c2.g*p.y + c3.g*p.z);
    out.b = (u8)(c1.b*p.x + c2.b*p.y + c3.b*p.z);
    out.a = (u8)(c1.a*p.x + c2.a*p.y + c3.a*p.z);
    return out;
}

#endif //_COLOUR_H
