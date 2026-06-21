#include "render.h"

render_state_t render_state_init() {
    SDL_Init(SDL_INIT_VIDEO);

    render_state_t render_state;

    render_state.window = SDL_CreateWindow(
        "SPARRY",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    printf("window: %p\n", render_state.window);
    SDL_RaiseWindow(render_state.window);

    render_state.renderer = SDL_CreateRenderer(render_state.window, -1, SDL_RENDERER_ACCELERATED);
    printf("renderer: %p\n", render_state.renderer);

    render_state.texture = SDL_CreateTexture(render_state.renderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    return render_state;
}

void render_state_update(render_state_t *render_state)
{
    SDL_UpdateTexture(render_state->texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
    SDL_RenderCopy(render_state->renderer, render_state->texture, NULL, NULL);
    SDL_RenderPresent(render_state->renderer);
}

void render_state_destroy(render_state_t *render_state)
{
    SDL_DestroyRenderer(render_state->renderer);
    SDL_DestroyWindow(render_state->window);
}

void put_pixel(i32 x, i32 y, i32 z, u32 c)
{
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (idx < 0 || idx >= SCREEN_HEIGHT*SCREEN_WIDTH) {
        return;
    }
    if (!zbuffer[idx] || zbuffer[idx]> z) {
        zbuffer[idx] = z;
        framebuffer[idx] = c;
    }
}

void put_pixel_vec(v3 v, u32 c)
{
    v.x = (i32)v.x;
    v.y = (i32)v.y;
    if (v.x < 0 || v.x >= SCREEN_WIDTH || v.y < 0 || v.y >= SCREEN_HEIGHT) return;

    u32 idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!zbuffer[idx] || zbuffer[idx]> v.z) {
        zbuffer[idx] = v.z;
        framebuffer[idx] = c;
    }
}

