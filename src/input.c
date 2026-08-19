#include "input.h"
#include "common.h"

#include <SDL2/SDL.h>

// TODO: update to frame independant dtime
#define DELTA_MOVE 0.3f
#define DELTA_LOOK 0.3f

#define PITCH_LIMIT_DEG 85.0f
#define PITCH_LIMIT_RAD  DEG_TO_RAD(PITCH_LIMIT_DEG)

static f32 pitch = 0.0f;
static f32 yaw = 0.0f;

void input_process(const uint8_t* keystate)
{
    if (keystate[SDL_SCANCODE_LEFT])  { yaw -= DEG_TO_RAD(DELTA_LOOK); }
    if (keystate[SDL_SCANCODE_RIGHT]) { yaw += DEG_TO_RAD(DELTA_LOOK); }
    if (keystate[SDL_SCANCODE_UP])    { pitch += DEG_TO_RAD(DELTA_LOOK); }
    if (keystate[SDL_SCANCODE_DOWN])  { pitch -= DEG_TO_RAD(DELTA_LOOK); }

    if (pitch > PITCH_LIMIT_RAD)  pitch = PITCH_LIMIT_RAD;
    if (pitch < -PITCH_LIMIT_RAD) pitch = -PITCH_LIMIT_RAD;

    v3 dir = (v3){
        .x = cosf(pitch) * sinf(yaw),
        .y = sinf(pitch),
        .z = cosf(pitch) * cosf(yaw),
    };
    v3 right = v3_norm(v3_cross(g_camera.up, dir));

    if (keystate[SDL_SCANCODE_W]) {
        g_camera.pos = v3_add(g_camera.pos, v3_scale(dir, DELTA_MOVE));
    } 
    if (keystate[SDL_SCANCODE_S]) {
        g_camera.pos = v3_sub(g_camera.pos, v3_scale(dir, DELTA_MOVE));
    } 
    if (keystate[SDL_SCANCODE_A]) {
        g_camera.pos = v3_sub(g_camera.pos, v3_scale(right, DELTA_MOVE));
    } 
    if (keystate[SDL_SCANCODE_D]) {
        g_camera.pos = v3_add(g_camera.pos, v3_scale(right, DELTA_MOVE));
    }
    if (keystate[SDL_SCANCODE_SPACE])  g_camera.pos.y += DELTA_MOVE; 
    if (keystate[SDL_SCANCODE_LSHIFT]) g_camera.pos.y -= DELTA_MOVE;

    g_camera.look_at = v3_add(g_camera.pos, dir);

    if (keystate[SDL_SCANCODE_F1])
        {g_snapshot_mode = true;}
}
