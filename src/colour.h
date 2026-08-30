#ifndef _COLOUR_H
#define _COLOUR_H

#include "la.h"

typedef struct colour_s {
    u8 r, g, b, a;
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
