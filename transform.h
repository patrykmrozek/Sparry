#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "camera.h"
#include "la.h"

extern camera_t g_camera;

void to_view_space(v3 *_v);
void to_ndc(v3 *_v);
void to_screen(v3 *v);
v3 *world_to_screen(v3 *v);

#endif
