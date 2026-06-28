#ifndef _LA_H
#define _LA_H

#include <math.h>
#include <stdio.h>
#include "common.h"

#define M4_ID (m4){{   \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}}}

#define M4_TRANS(_t) (m4){{       \
    {1, 0, 0, _t.x}, \
    {0, 1, 0, _t.y}, \
    {0, 0, 1, _t.z}, \
    {0, 0, 0,  1}}}

#define M4_SCALE(_s) (m4){{  \
    {_s,     0,     0,   0}, \
    {  0,   _s,     0,   0}, \
    {  0,     0,   _s,   0}, \
    {  0,     0,    0,   1}}}

#define M4_SCALEV(_s) (m4){{   \
    {_s.x,     0,     0,   0}, \
    {  0,   _s.y,     0,   0}, \
    {  0,     0,   _s.z,   0}, \
    {  0,     0,     0,    1}}} 

#define M4_ROTX(_a) (m4){{     \
    {1,       0,       0, 0}, \
    {0, cos(_a), -sin(_a), 0}, \
    {0, sin(_a), cos(_a), 0}, \
    {0,       0,       0, 1}}}

#define M4_ROTY(_a) (m4){{     \
    { cos(_a), 0, sin(_a), 0}, \
    {       0, 1,        0, 0}, \
    {-sin(_a), 0,  cos(_a), 0}, \
    {       0, 0,        0, 1}}}

#define M4_ROTZ(_a) (m4){{     \
    {cos(_a), -sin(_a), 0, 0}, \
    {sin(_a),  cos(_a), 0, 0}, \
    {       0,       0, 1, 0}, \
    {       0,       0, 0, 1}}}

typedef struct v3_s {
    f32 x,y,z;
} v3;
static inline v3 v3_init(f32 x, f32 y, f32 z)
{
    return (v3){
        .x=x,
        .y=y,
        .z=z
    };
}
static inline void v3_print(v3 v)
{
    printf("{ %f, %f, %f }\n", v.x, v.y, v.z);
}
static inline v3 v3_fill(f32 f)
{
    return (v3){f, f, f};
}
static inline v3 v3_add(v3 v1, v3 v2)
{
    return (v3){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}
static inline v3 v3_sub(v3 v1, v3 v2)
{
    return (v3){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}
static inline v3 v3_scale(v3 v, f32 s)
{
    return (v3){v.x*s, v.y*s, v.z*s};
}
static inline v3 v3_mul(v3 v1, v3 v2)
{
   return (v3){v1.x*v2.x, v1.y*v2.y, v1.z*v2.z}; 
}
static inline v3 v3_div(v3 v1, v3 v2)
{
    return (v3){v1.x/v2.x, v1.y/v2.y, v1.z/v2.z};
}
static inline f32 v3_dot(v3 v1, v3 v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z));
}
static inline v3 v3_cross(v3 v1, v3 v2)
{
    return (v3){
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x),
    };
}
static inline f32 v3_len(v3 v)
{
    return (sqrtf(v3_dot(v, v)));
}
static inline v3 v3_norm(v3 v)
{
    f32 l = v3_len(v);
    return (v3)v3_div(v, (v3){l, l, l});
}
static inline v3 v3_neg(v3 v)
{
    return (v3){-v.x, -v.y, -v.z};
}

typedef struct v4_s {
    f32 x,y,z,w;
} v4;
typedef struct m4_s {
    f32 v[4][4]; //[row][col]
} m4;
static inline v4 v3_to_v4(v3 v)
{
    return (v4){v.x, v.y, v.z, 1};
}
static inline v4 v4_norm(v4 v)
{
    return (v4){v.x/v.w, v.y/v.w, v.z/v.w, v.w/v.w}; 
}
static inline void v4_print(v4 v)
{
    printf("{ %f, %f, %f, %f }\n", v.x, v.y, v.z, v.w);
}
static inline v3 v4_to_v3(v4 v)
{
    if (v.w != 1) v = v4_norm(v);
    return (v3){v.x, v.y, v.z};
}
static inline f32 v4_dot(v4 v1, v4 v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w*v2.w));
}
static inline v4 m4_v4_mul(m4 m, v4 v)
{
    f32 out[4];
    for (int row = 0; row < 4; row++) {
        out[row] = v4_dot((v4){m.v[row][0], m.v[row][1], m.v[row][2], m.v[row][3]}, v);
    }
    return (v4){out[0], out[1], out[2], out[3]};
}
static inline m4 m4_mul(m4 m1, m4 m2)
{
    m4 out;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out.v[i][j] = v4_dot(
                (v4){m1.v[i][0], m1.v[i][1], m1.v[i][2], m1.v[i][3]},
                (v4){m2.v[0][j], m2.v[1][j], m2.v[2][j], m2.v[3][j]});
        }
    }
    return out;
}
static inline void m4_print(m4 m)
{
    for (int row = 0; row < 4; row++) {
        v4_print((v4){m.v[row][0], m.v[row][1], m.v[row][2], m.v[row][3]});
    }
}
static inline v4 v4_trans(v4 v, v3 t)
{
    return m4_v4_mul(M4_TRANS(t), v);
}
static inline v3 v3_trans(v3 v, v3 t)
{
    return v4_to_v3(v4_trans(v3_to_v4(v), t));
}
static inline v4 v4_scale(v4 v, f32 s)
{
    return m4_v4_mul(M4_SCALE(s), v);
}
static inline v4 v4_scalev(v4 v, v3 s)
{
    return m4_v4_mul(M4_SCALEV(s), v);
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

#endif //_LA_H
