#include "input.h"

#include <SDL2/SDL.h>

f32 delta = 0.1f;

void input_process(const uint8_t* keystate)
{
    if (keystate[SDL_SCANCODE_W]) { g_camera.pos.z += delta; g_camera.look_at.z += delta; }
    if (keystate[SDL_SCANCODE_S]) { g_camera.pos.z -= delta; g_camera.look_at.z -= delta; }
    if (keystate[SDL_SCANCODE_A]) { g_camera.pos.x -= delta; g_camera.look_at.x -= delta; }
    if (keystate[SDL_SCANCODE_D]) { g_camera.pos.x += delta; g_camera.look_at.x += delta; }
    if (keystate[SDL_SCANCODE_SPACE]) { g_camera.pos.y += delta; g_camera.look_at.y += delta; }
    if (keystate[SDL_SCANCODE_LSHIFT]) { g_camera.pos.y -= delta; g_camera.look_at.y -= delta; }
    if (keystate[SDL_SCANCODE_LEFT]) { g_camera.look_at.x -= delta; }
    if (keystate[SDL_SCANCODE_RIGHT]) { g_camera.look_at.x += delta; }
    if (keystate[SDL_SCANCODE_UP]) { g_camera.look_at.y -= delta; }
    if (keystate[SDL_SCANCODE_DOWN]) { g_camera.look_at.y += delta; }
}
