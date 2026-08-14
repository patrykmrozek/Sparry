#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CELP_IMPLEMENTATION

#include "raster.h"
#include "result.h"
#include "common.h"
#include "render.h"
#include "input.h"

int main() 
{
    INFO("SPARRY\n");

    rd_state_t *rd_state;
    result_t res = rd_state_init(&rd_state);
    if (res != RESULT_OK) {
        ERROR("main render state creation failed");
        return 1;
    }
    
    v3 b1 = {-10, -10,  10};
    v3 b2 = { 10, -10,  10};
    v3 b3 = { 10, -10, -10};
    v3 b4 = {-10, -10, -10};

    v3 t1 = {-10, 10,  10};
    v3 t2 = { 10, 10,  10};
    v3 t3 = { 10, 10, -10};
    v3 t4 = {-10, 10, -10};

    SDL_Event event;
    bool game_running = 1;
    const u8* keystate = SDL_GetKeyboardState(NULL);

    u32 c = RGBA_TO_HEX(255, 255, 255, 1);

    while (game_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

        input_process(keystate);
        rd_frame_begin(rd_state);
        {
            rt_put_tri(rd_state->rt_ctx, t1, b1, b2, c);
            rt_put_tri(rd_state->rt_ctx, t2, t1, b2, c);
        }
        rd_frame_end(rd_state); 
    }

    rd_state_destroy(rd_state);
    return 0;
}
