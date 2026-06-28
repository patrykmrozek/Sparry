#include "render.h"

render_state_t *render_state_init() 
{
    SDL_Init(SDL_INIT_VIDEO);

    render_state_t *render_state = malloc(sizeof(render_state_t));

    render_state->window = SDL_CreateWindow(
        "SPARRY",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    printf("window: %p\n", render_state->window);
    SDL_RaiseWindow(render_state->window);

    render_state->renderer = SDL_CreateRenderer(
        render_state->window,
        -1,
        SDL_RENDERER_ACCELERATED);
    printf("renderer: %p\n", render_state->renderer);

    render_state->texture = SDL_CreateTexture(
        render_state->renderer,
        SDL_PIXELFORMAT_BGRA32,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
    printf("texture: %p\n", render_state->texture);

    return render_state;
}

void render_state_destroy(render_state_t *render_state)
{
    SDL_DestroyRenderer(render_state->renderer);
    SDL_DestroyWindow(render_state->window);
    free(render_state);
}


void render_frame_begin(render_state_t *render_state)
{
    memset(render_state->framebuffer, 0, sizeof(render_state->framebuffer));

    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        render_state->zbuffer[i] = FLT_MAX;
    }
}

void render_frame_end(render_state_t *render_state)
{
    SDL_UpdateTexture(
            render_state->texture,
            NULL, 
            render_state->framebuffer, 
            SCREEN_WIDTH * sizeof(u32));
    SDL_RenderCopy(
            render_state->renderer,
            render_state->texture,
            NULL,
            NULL);
    SDL_RenderPresent(
            render_state->renderer);
}

void put_pixel(render_state_t *render_state, i32 x, i32 y, i32 z, u32 c)
{
    u32 idx;

    idx = (y * SCREEN_WIDTH) + x;
        if (idx < 0 || idx >= SCREEN_HEIGHT*SCREEN_WIDTH) {
            return;
        }
        if (!render_state->zbuffer[idx] || render_state->zbuffer[idx]> z) {
            render_state->zbuffer[idx] = z;
            render_state->framebuffer[idx] = c;
    }
}

void put_pixel_vec(render_state_t *render_state, v3 v, u32 c)
{
    u32 idx;

    v.x = (i32)v.x;
    v.y = (i32)v.y;
    if (v.x < 0 || v.x >= SCREEN_WIDTH || v.y < 0 || v.y >= SCREEN_HEIGHT) return;

    idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!render_state->zbuffer[idx] || render_state->zbuffer[idx]> v.z) {
        render_state->zbuffer[idx] = v.z;
        render_state->framebuffer[idx] = c;
    }
}

