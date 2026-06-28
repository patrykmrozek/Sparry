#ifndef _RENDERER_H
#define _RENDERER_H

#include "raster.h"

typedef struct render_state_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture; 

    raster_context_t *raster_ctx;
} render_state_t;

render_state_t *render_state_init();
void render_state_update(render_state_t *render_state);
void render_state_destroy(render_state_t *render_state);
void render_frame_begin(render_state_t *render_state);
void render_frame_end(render_state_t *render_state);

#endif //_RENDER_H
