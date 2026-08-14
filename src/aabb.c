#include "aabb.h"

aabb_t aabb_get(v3 p1, v3 p2, v3 p3)
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
