#ifndef _INPUT_H
#define _INPUT_H

#include "camera.h"

extern camera_t g_camera;

void input_process(const uint8_t* keystate);

#endif //_INPUT_H
