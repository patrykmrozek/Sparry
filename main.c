#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "render.h"
#include "transform.h"
#include "input.h"

int main() 
{
    printf("SPARRY\n");
    SDL_Event event;
    bool game_running = 1;

    render_state_t *render_state = render_state_init();

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

    v3 p1 = {-10, -10, 0};
    v3 p2 = { 10, -10, 0};
    v3 p3 = {-10,  10, 0};
    v3 p4 =  {10,  10, 0};
    v3 ps[4] = {p1, p2, p3, p4};


    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

        input_process(keystate);
        render_frame_begin(render_state);

        for (int i = 0; i < 4; i++) {
            v3 p;
            if  (world_to_screen(ps[i], &p)) {
                put_pixel_vec(render_state, p, 0xFFFFFFFF);
            }
        }
        
       render_frame_end(render_state); 
    }

    render_state_destroy(render_state);
    return 0;
}
