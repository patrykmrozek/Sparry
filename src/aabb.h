#ifndef _AABB_H
#define _AABB_H

#include "la.h"

typedef struct aabb_s {
    v3 min, max;
} aabb_t;

aabb_t aabb_get(v3 p1, v3 p2, v3 p3);

#endif //_AABB_H
