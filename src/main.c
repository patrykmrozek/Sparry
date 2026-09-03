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

static inline void
render_multicolor_cube(render_state_t *rdstate, f32 size)
{
    v3 bfl = {-size, -size,  size};
    v3 bfr = { size, -size,  size};
    v3 bcr = { size, -size, -size};
    v3 bcl = {-size, -size, -size};

    v3 tfl = {-size, size,  size};
    v3 tfr = { size, size,  size};
    v3 tcr = { size, size, -size};
    v3 tcl = {-size, size, -size};

    //front
    raster_tri3c(rdstate->raster_ctx, tcl, bcl, bcr,
                 COL_RED, COL_GREEN, COL_BLUE); 
    raster_tri(rdstate->raster_ctx, tcl, tcr, bcr, COL_GREEN); 
    //back
    raster_tri(rdstate->raster_ctx, tfl, bfl, bfr, COL_BLUE); 
    raster_tri(rdstate->raster_ctx, tfl, tfr, bfr, COL_YELLOW); 
    //bottom
    raster_tri(rdstate->raster_ctx, bfl, bcl, bcr, COL_PURPLE); 
    raster_tri(rdstate->raster_ctx, bfl, bfr, bcr, COL_WHITE); 
    //top
    raster_tri(rdstate->raster_ctx, tfl, tcl, tcr, COL_BLACK); 
    raster_tri(rdstate->raster_ctx, tfl, tfr, tcr, COL_LBLUE);
    //left
    raster_tri(rdstate->raster_ctx, tfl, bfl, bcl, COL_ORANGE); 
    raster_tri(rdstate->raster_ctx, tfl, tcl, bcl, COL_PINK); 
    //right
    raster_tri(rdstate->raster_ctx, tfr, bfr, bcr, COL_LGREY); 
    raster_tri(rdstate->raster_ctx, tfr, tcr, bcr, COL_DGREY);   
}

int main() 
{
    INFO("SPARRY\n");

    render_state_t *render_state;
    result_t res = render_state_init(&render_state);
    HANDLE_ERROR_RET(res!=RESULT_OK, 1, NULL);
    
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

        render_frame_begin(render_state);
        {
            render_multicolor_cube(render_state, 15);
            raster_line(render_state->raster_ctx,
                        lp0, lp1, COL_RED);
        }
        render_frame_end(render_state);

    }

    render_state_destroy(render_state);
    return 0;
}
