#ifndef _COMMON_H
#define _COMMON_H

#define CELP_MATH
#define CELP_STRIP_PREFIX
#include "../../celp/celp.h"
#undef log

extern bool g_snapshot_mode;

#ifdef SNAPSHOT_MODE
    #undef DEBUG 
    #undef TRACE
    #define DEBUG(lvl, fmt, ...) \
    do { \
        if (g_snapshot_mode) { \
            CELP_DEBUG(lvl, fmt, ##__VA_ARGS__); \
        } \
    } while(0)

    #define TRACE(lvl, fmt, ...) \
    do { \
        if (g_snapshot_mode) { \
            CELP_TRACE(lvl, fmt, ##__VA_ARGS__); \
        } \
    } while(0)
#endif //SNAPSHOT_MODE

#define PI 3.14159265359f

#define NEAR 1.0f
#define FAR 100000.0f
#define FOV DEG_TO_RAD(90) 

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define DEG_TO_RAD(_d) ((_d) * (PI / 180)) 
#define RAD_TO_DEG(_d) ((_d) * (180 / PI))

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

#define IN_BOUNDS(_x, _y) \
    ((_x) >= 0 && \
     (_x) < SCREEN_WIDTH && \
     (_y) >= 0 && \
     (_y) < SCREEN_HEIGHT)

#define HANDLE_ERROR_RET(cond, err, msg) \
({ \
    if ((cond)) { \
        if ((msg)) ERROR((msg)); else ERROR(""); \
        return err; \
    } \
 })

#define HANDLE_ERROR_TAG(cond, err, msg, res, tag) \
({ \
    if ((cond)) { \
        if ((msg)) ERROR((msg)); else ERROR(""); \
        (res) = (err); \
        goto tag; \
    } \
 })

#endif //_COMMON_H
