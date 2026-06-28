#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2/SDL.h>

#include "la.h"

typedef struct render_state_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
    f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
} render_state_t;

render_state_t *render_state_init();
void render_state_update(render_state_t *render_state);
void render_state_destroy(render_state_t *render_state);
void render_frame_begin(render_state_t *render_state);
void render_frame_end(render_state_t *render_state);
void put_pixel(render_state_t *render_state, i32 x, i32 y, i32 z, u32 c);
void put_pixel_vec(render_state_t *render_state, v3 v, u32 c);

#endif //_RENDER_H
