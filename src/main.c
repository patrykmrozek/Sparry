#include "result.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CELP_IMPLEMENTATION

#include "common.h"
#include "render.h"
#include "input.h"

int main() 
{
    printf("SPARRY\n");

    render_state_t *render_state;
    result_t res = render_state_init(&render_state);
    if (res != RESULT_OK) {
        ERROR("main render state creation failed");
        return 1;
    }
    
    v3 b1 = {-10, -10,  10};
    v3 b2 = { 10, -10,  10};
    v3 b3 = { 10, -10, -10};
    v3 b4 = {-10, -10, -10};

    v3 t1 = {-10,  10,  10};
    v3 t2 = { 10,  10,  10};
    v3 t3 = { 10,  10, -10};
    v3 t4 = {-10,  10, -10};

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
            raster_put_line(render_state->raster_ctx, b1, b2, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b2, b3, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b3, b4, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b4, b1, 0xFFFFFFFF);

            raster_put_line(render_state->raster_ctx, t1, t2, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, t2, t3, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, t3, t4, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, t4, t1, 0xFFFFFFFF);

            raster_put_line(render_state->raster_ctx, b1, t1, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b2, t2,  0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b3, t3, 0xFFFFFFFF);
            raster_put_line(render_state->raster_ctx, b4, t4, 0xFFFFFFFF);
        }
        render_frame_end(render_state); 
    }

    render_state_destroy(render_state);
    return 0;
}
