#include "input.h"

#include <SDL2/SDL.h>

f32 delta_move = 0.3f;
f32 delta_look = 0.7f;

void input_process(const uint8_t* keystate)
{
    if (keystate[SDL_SCANCODE_W]) { g_camera.pos.z += delta_move; g_camera.look_at.z += delta_move; }
    if (keystate[SDL_SCANCODE_S]) { g_camera.pos.z -= delta_move; g_camera.look_at.z -= delta_move; }
    if (keystate[SDL_SCANCODE_A]) { g_camera.pos.x -= delta_move; g_camera.look_at.x -= delta_move; }
    if (keystate[SDL_SCANCODE_D]) { g_camera.pos.x += delta_move; g_camera.look_at.x += delta_move; }
    if (keystate[SDL_SCANCODE_SPACE]) { g_camera.pos.y += delta_move; g_camera.look_at.y += delta_move; }
    if (keystate[SDL_SCANCODE_LSHIFT]) { g_camera.pos.y -= delta_move; g_camera.look_at.y -= delta_move; }
    if (keystate[SDL_SCANCODE_LEFT]) { g_camera.look_at.x -= delta_look; }
    if (keystate[SDL_SCANCODE_RIGHT]) { g_camera.look_at.x += delta_look; }
    if (keystate[SDL_SCANCODE_UP]) { g_camera.look_at.y += delta_look; }
    if (keystate[SDL_SCANCODE_DOWN]) { g_camera.look_at.y -= delta_look; }
}
