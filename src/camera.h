#ifndef _CAMERA_H
#define _CAMERA_H

#include <stdio.h>
#include "la.h"

typedef struct camera_s {
    v3 pos;
    v3 up;
    v3 look_at;
} camera_t;

#endif //_CAMERA_H
