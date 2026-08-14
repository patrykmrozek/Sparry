#ifndef _LA_H
#define _LA_H

#include <math.h>
#include <stdio.h>
#include "common.h"

V2(f32)
#define v2 V2_T(f32)

V3(f32)
#define v3 V3_T(f32)

V4(f32)
#define v4 V4_T(f32)

M4(f32)
#define m4 M4_T(f32)

static inline void v4_print(v4 v)
{
    printf("{ %f, %f, %f, %f }\n", v.x, v.y, v.z, v.w);
}

static inline void m4_print(m4 m)
{
    for (int row = 0; row < 4; row++) {
        v4_print((v4){m.v[row][0], m.v[row][1], m.v[row][2], m.v[row][3]});
    }
}
static inline f32 lerp(f32 a, f32 b, f32 t)
{
    return a + (b - a) * t;
}
static inline v3 v3_lerp(v3 a, v3 b, f32 t)
{
    return (v3){
        .x = lerp(a.x, b.x, t),
        .y = lerp(a.y, b.y, t),
        .z = lerp(a.z, b.z, t)
    };
}

#define TRI_AREA(a, b, c) (v2_cross(v2_sub(b, a), v2_sub(c, a)))

#define V3_TO_V2(v) (v2){v.x, v.y}

//a barycentric coord $ in triangle A, B, C could be described 
//as the ratio of areas $BC, $CA, $AB
//if any are negative, $ is outside of the triangle
static inline v3 barycentric(v3 a, v3 b, v3 c, v3 p)
{
    DEBUG("a: "V3F_STR(a));
    DEBUG("b: "V3F_STR(b));
    DEBUG("c: "V3F_STR(c));
    v2 a2 = V3_TO_V2(a);
    v2 b2 = V3_TO_V2(b);
    v2 c2 = V3_TO_V2(c);
    v2 p2 = V3_TO_V2(p);
    //tri areas
    f32 tri_area = TRI_AREA(a2, b2, c2);
    DEBUG("tri_area: %f", tri_area);
    f32 pbc = TRI_AREA(p2, b2, c2)/tri_area;
    f32 pca = TRI_AREA(p2, c2, a2)/tri_area;
    f32 pab = 1 - pbc - pca;

    return (v3){pbc, pca, pab};
}

#endif //_LA_H
