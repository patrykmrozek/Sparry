#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "render.h"
#include "transform.h"

int main() 
{
    printf("SPARRY\n");
    SDL_Event event;
    bool game_running = 1;

    render_state_t renderer = render_state_init();

    m4 m = (m4){{
        {2, 0, 0, 0},
        {0, 2, 0, 0},
        {0, 0, 2, 0},
        {1, 2, 3, 1},
    }};
    m4 res = m4_mul(m, M4_ID);
    m4_print(res);
    printf("\n");
    res = m4_mul(M4_ID, m);
    m4_print(res);

    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
            if (keystate[SDL_SCANCODE_W]) g_camera.pos.z += 1.0f;
            if (keystate[SDL_SCANCODE_S]) g_camera.pos.z -= 1.0f;
            if (keystate[SDL_SCANCODE_A]) g_camera.pos.x -= 1.0f;
            if (keystate[SDL_SCANCODE_D]) g_camera.pos.x += 1.0f;
            if (keystate[SDL_SCANCODE_SPACE]) g_camera.pos.y += 1.0f;
            if (keystate[SDL_SCANCODE_LSHIFT]) g_camera.pos.y -= 1.0f;
        }
        /*
        for (int i = 0; i < 100; i++) {
            framebuffer[10000+i] = 0xFFFF0000;
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+i, 5, 0xFF00FF00);
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+100-i, 10, 0xFF0000FF);
            put_pixel_vec((v3){(SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+50, 7}, 0xFFFFFF00);
        }
        */

        memset(framebuffer, 0, sizeof(framebuffer));
        memset(zbuffer, 0, sizeof(zbuffer));

        v3 p1 = {-10, -10, 0};
        v3 p2 = { 10, -10, 0};
        v3 p3 = {-10,  10, 0};
        v3 p4 =  {10,  10, 0};
        v3 ps[4] = {p1, p2, p3, p4};

        for (int i = 0; i < 4; i++) {
            v3 p = world_to_screen(ps[i]);
            //printf("P[%d]: ", i);
            printf("\n");
            //v3_print(ps[i]);
            put_pixel_vec(p, 0xFFFFFFFF);
        }
        
        render_state_update(&renderer);
        }

    render_state_destroy(&renderer);
    return 0;
}
