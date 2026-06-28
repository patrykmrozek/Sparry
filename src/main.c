#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "render.h"
#include "input.h"

int main() 
{
    printf("SPARRY\n");

    render_state_t *render_state = render_state_init();
    
    v3 bp1 = {-5, 5, 1};
    v3 bp2 = {5, 5, 2};

    v3 tp1 = {-5, -5, 3};
    v3 tp2 = {5, -5, 4};

    v3 lp1 = {-5, -5, 5};
    v3 lp2 = {-5, 5, 6};

    v3 rp1 = {5, -5, 7};
    v3 rp2 = {5, 5, 8};

    SDL_Event event;
    bool game_running = 1;

    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

        input_process(keystate);
        render_frame_begin(render_state);
        {
            raster_put_line(render_state->raster_ctx, bp1, bp2, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, tp1, tp2, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, lp1, lp2, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, rp1, rp2, 0xFFFFFFFF);
        }
        render_frame_end(render_state); 
    }

    render_state_destroy(render_state);
    return 0;
}
