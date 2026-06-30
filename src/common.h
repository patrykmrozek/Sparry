#ifndef _COMMON_H
#define _COMMON_H

#define CELP_STRIP_PREFIX
#include "../include/celp.h"
#undef log
#define LOG celp_log

#include <stdint.h>

#define PI 3.14159265359f

#define NEAR 1.0f
#define FAR 100000.0f
#define FOV DEG_TO_RAD(90) 

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define DEG_TO_RAD(_d) ((_d) * (PI / 180)) 
#define RAD_TO_DEG(_d) ((_d) * (180 / PI))

#define IN_BOUNDS(_x, _y) ((_x) >= 0 && (_x) < SCREEN_WIDTH && (_y) >= 0 && (_y) < SCREEN_HEIGHT)

typedef uint32_t u32;
typedef int i32;
typedef float f32;

#endif //_COMMON_H
