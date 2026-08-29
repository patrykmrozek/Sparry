#include "input.h"
#include "common.h"

#define DELTA_MOVE_FACTOR 100.0f
#define DELTA_LOOK_FACTOR 100.0f

#define PITCH_LIMIT_DEG 85.0f
#define PITCH_LIMIT_RAD DEG_TO_RAD(PITCH_LIMIT_DEG)

static f32 pitch = 0.0f;
static f32 yaw   = 0.0f;

void input_process()
{
    const u8* keystate = SDL_GetKeyboardState(NULL);

    f64 delta_time = get_delta_time();
    f64 delta_look = delta_time * DELTA_LOOK_FACTOR;
    f64 delta_move = delta_time * DELTA_MOVE_FACTOR;

    if (keystate[SDL_SCANCODE_LEFT])  { yaw -= DEG_TO_RAD(delta_look); }
    if (keystate[SDL_SCANCODE_RIGHT]) { yaw += DEG_TO_RAD(delta_look); }
    if (keystate[SDL_SCANCODE_UP])    { pitch += DEG_TO_RAD(delta_look); }
    if (keystate[SDL_SCANCODE_DOWN])  { pitch -= DEG_TO_RAD(delta_look); }

    if (pitch > PITCH_LIMIT_RAD)  pitch = PITCH_LIMIT_RAD;
    if (pitch < -PITCH_LIMIT_RAD) pitch = -PITCH_LIMIT_RAD;

    v3 dir = (v3){
        .x = cosf(pitch) * sinf(yaw),
        .y = sinf(pitch),
        .z = cosf(pitch) * cosf(yaw),
    };
    v3 right = v3_norm(v3_cross(g_camera.up, dir));

    if (keystate[SDL_SCANCODE_W]) {
        g_camera.pos = v3_add(g_camera.pos, v3_scale(dir, delta_move));
    } 
    if (keystate[SDL_SCANCODE_S]) {
        g_camera.pos = v3_sub(g_camera.pos, v3_scale(dir, delta_move));
    } 
    if (keystate[SDL_SCANCODE_A]) {
        g_camera.pos = v3_sub(g_camera.pos, v3_scale(right, delta_move));
    } 
    if (keystate[SDL_SCANCODE_D]) {
        g_camera.pos = v3_add(g_camera.pos, v3_scale(right, delta_move));
    }
    if (keystate[SDL_SCANCODE_SPACE])  g_camera.pos.y += delta_move; 
    if (keystate[SDL_SCANCODE_LSHIFT]) g_camera.pos.y -= delta_move;

    g_camera.look_at = v3_add(g_camera.pos, dir);

    if (keystate[SDL_SCANCODE_F1])
        {g_snapshot_mode = true;}
}
