#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CELP_IMPLEMENTATION

#include "raster.h"
#include "render.h"
#include "result.h"
#include "input.h"

bool g_snapshot_mode = false;

int main() 
{
    INFO("SPARRY\n");

    rd_state_t *rd_state;
    result_t res = rd_state_init(&rd_state);
    HANDLE_ERROR_RET(res!=RESULT_OK, 1, NULL);
    
    v3 bfl = {-10, -10,  10};
    v3 bfr = { 10, -10,  10};
    v3 bcr = { 10, -10, -10};
    v3 bcl = {-10, -10, -10};

    v3 tfl = {-10, 10,  10};
    v3 tfr = { 10, 10,  10};
    v3 tcr = { 10, 10, -10};
    v3 tcl = {-10, 10, -10};

    SDL_Event event;
    bool game_running = 1;

    colour_t c = (colour_t){255, 255, 255, 255};

    while (game_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

        g_snapshot_mode = false;
        input_process();
        rd_frame_begin(rd_state);
        {
            //front
            rt_put_tri(rd_state->rt_ctx, tcl, bcl, bcr, c); 
            rt_put_tri(rd_state->rt_ctx, tcl, tcr, bcr, c); 
            //back
            rt_put_tri(rd_state->rt_ctx, tfl, bfl, bfr, c); 
            rt_put_tri(rd_state->rt_ctx, tfl, tfr, bfr, c); 
            //bottom
            rt_put_tri(rd_state->rt_ctx, bfl, bcl, bcr, c); 
            rt_put_tri(rd_state->rt_ctx, bfl, bfr, bcr, c); 
            //top
            rt_put_tri(rd_state->rt_ctx, tfl, tcl, tcr, c); 
            rt_put_tri(rd_state->rt_ctx, tfl, tfr, tcr, c);
            //left
            rt_put_tri(rd_state->rt_ctx, tfl, bfl, bcl, c); 
            rt_put_tri(rd_state->rt_ctx, tfl, tcl, bcl, c); 
            //right
            rt_put_tri(rd_state->rt_ctx, tfr, bfr, bcr, c); 
            rt_put_tri(rd_state->rt_ctx, tfr, tcr, bcr, c); 
        }
        rd_frame_end(rd_state); 
    }

    rd_state_destroy(rd_state);
    return 0;
}
