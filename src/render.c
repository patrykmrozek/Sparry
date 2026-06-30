#include "render.h"
#include "raster.h"

render_state_t *render_state_init() 
{
    SDL_Init(SDL_INIT_VIDEO);

    render_state_t *render_state = 
        (render_state_t*)malloc(sizeof(render_state_t));

    render_state->window = 
        SDL_CreateWindow("SPARRY",
                         SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED,
                         SCREEN_WIDTH,
                         SCREEN_HEIGHT,
                         SDL_WINDOW_SHOWN);
    LOG(LOG_LEVEL_INFO, "window: %p\n", render_state->window);
    SDL_RaiseWindow(render_state->window);
    
    render_state->renderer = 
        SDL_CreateRenderer(render_state->window,
                           -1,
                           SDL_RENDERER_ACCELERATED);
    LOG(LOG_LEVEL_INFO, "renderer: %p\n", render_state->renderer);

    render_state->texture = 
        SDL_CreateTexture(render_state->renderer,
                          SDL_PIXELFORMAT_BGRA32,
                          SDL_TEXTUREACCESS_STREAMING,
                          SCREEN_WIDTH,
                          SCREEN_HEIGHT);
    LOG(LOG_LEVEL_INFO, "texture: %p\n", render_state->texture);

    render_state->raster_ctx = raster_context_init();

    return render_state;
}

void render_state_destroy(render_state_t *render_state)
{
    SDL_DestroyRenderer(render_state->renderer);
    SDL_DestroyWindow(render_state->window);
    raster_context_destroy(render_state->raster_ctx);
    free(render_state);
}


void render_frame_begin(render_state_t *render_state)
{
   raster_context_clear(render_state->raster_ctx); 
}

void render_frame_end(render_state_t *render_state)
{
    SDL_UpdateTexture(render_state->texture,
                      NULL, 
                      render_state->raster_ctx->framebuffer, 
                      SCREEN_WIDTH * sizeof(u32));
    SDL_RenderCopy(render_state->renderer,
                   render_state->texture,
                   NULL,
                   NULL);
    SDL_RenderPresent(render_state->renderer);

}

