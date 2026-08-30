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

    v3 lp0 = {-20, 20, 20};
    v3 lp1 = { 20, -20, -20};

    SDL_Event event;
    bool game_running = 1;

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
            rt_tri3c(rd_state->rt_ctx, tcl, bcl, bcr,
                    COL_RED, COL_GREEN, COL_BLUE); 
            rt_tri(rd_state->rt_ctx, tcl, tcr, bcr, COL_GREEN); 
            //back
            rt_tri(rd_state->rt_ctx, tfl, bfl, bfr, COL_BLUE); 
            rt_tri(rd_state->rt_ctx, tfl, tfr, bfr, COL_YELLOW); 
            //bottom
            rt_tri(rd_state->rt_ctx, bfl, bcl, bcr, COL_PURPLE); 
            rt_tri(rd_state->rt_ctx, bfl, bfr, bcr, COL_WHITE); 
            //top
            rt_tri(rd_state->rt_ctx, tfl, tcl, tcr, COL_BLACK); 
            rt_tri(rd_state->rt_ctx, tfl, tfr, tcr, COL_LBLUE);
            //left
            rt_tri(rd_state->rt_ctx, tfl, bfl, bcl, COL_ORANGE); 
            rt_tri(rd_state->rt_ctx, tfl, tcl, bcl, COL_PINK); 
            //right
            rt_tri(rd_state->rt_ctx, tfr, bfr, bcr, COL_LGREY); 
            rt_tri(rd_state->rt_ctx, tfr, tcr, bcr, COL_DGREY);  
            //line
            rt_line(rd_state->rt_ctx, lp0, lp1, COL_RED);
        }
        rd_frame_end(rd_state); 
    }

    rd_state_destroy(rd_state);
    return 0;
}
