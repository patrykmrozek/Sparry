#ifndef _RENDERER_H
#define _RENDERER_H

#include "raster.h"
#include "result.h"

typedef struct rd_state_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture; 

    rt_ctx_t *rt_ctx;
} rd_state_t;

result_t rd_state_init(rd_state_t **state_pp); 
void rd_state_update(rd_state_t *state);
void rd_state_destroy(rd_state_t *state);
void rd_frame_begin(rd_state_t *state);
void rd_frame_end(rd_state_t *state);

#endif //_RENDER_H
