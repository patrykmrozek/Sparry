#ifndef _LA_H
#define _LA_H

#include <math.h>
#include <stdio.h>
#include "common.h"

#undef v2
#undef v3
#undef v4
#undef m4


celp_v2(f32);
#define v2 v2_t(f32)

celp_v2(i32);
#define v2i v2_t(i32)

celp_v3(f32);
#define v3 v3_t(f32)

celp_v4(f32);
#define v4 v4_t(f32)

celp_m4(f32);
#define m4 m4_t(f32)

#define V3_MAT_MUL(mat, v, T) \
    v4_to_v3(v4_norm(m4_v4_mul((mat), v3_to_v4((v), T))), T)

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
#define V3_TO_V2i(v) (v2i){(i32)v.x, (i32)v.y}

//a barycentric coord $ in triangle A, B, C could be described 
//as the ratio of areas $BC:$CA:$AB
static inline v3 barycentric(v2 a, v2 b, v2 c, v2i p)
{
    //tri areas
    f32 tri_area = TRI_AREA(a, b, c);
    DEBUG(1, "tri_area: (%f %f) | (%f %f) | (%f %f) = %f",
          a.x, a.y, b.x, b.y, c.x, c.y, tri_area);
    f32 pbc = TRI_AREA(p, b, c)/tri_area;
    f32 pca = TRI_AREA(p, c, a)/tri_area;
    f32 pab = 1 - pbc - pca;

    return (v3){pbc, pca, pab};
} 

#endif //_LA_H
