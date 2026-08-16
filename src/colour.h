#ifndef _COLOUR_H
#define _COLOUR_H

#include "common.h"

typedef struct colour_s {
    u8 r, g, b, a;
} colour_t;

#define COL_TO_HEX(col) (u32)((col).a<<24)|\
                        (u32)((col).r<<16)|\
                        (u32)((col).g<<8)| \
                        (u32)(col).b

#define BARY_COL(bary, col)   \
({ \
    colour_t _c; \
    _c.r = (col).r*(bary).x; \
    _c.g = (col).g*(bary).y; \
    _c.b = (col).b*(bary).z; \
    _c.a = (col).a; \
    \
    _c; \
})


#endif //_COLOUR_H
