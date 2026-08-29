#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <stdbool.h>

#include "camera.h"
#include "la.h"

extern camera_t g_camera;

v3 tf_view(v3 v);
v3 tf_ndc(v3 v);
v3 tf_screen(v3 v);
bool tf_world_to_screen(v3 v, v3 *ret);

#endif //_TRANSFORM_H
