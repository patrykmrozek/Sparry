#ifndef _RENDERER_H
#define _RENDERER_H

#include "raster.h"
#include "result.h"

typedef struct render_state_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture; 

    raster_ctx_t *raster_ctx;
} render_state_t;

result_t render_state_init(render_state_t **state_pp); 
void render_state_update(render_state_t *state);
void render_state_destroy(render_state_t *state);
void render_frame_begin(render_state_t *state);
void render_frame_end(render_state_t *state);

#endif //_RENDER_H
