#include "render.h"
#include "raster.h"
#include "result.h"

result_t render_state_init(render_state_t **out_render_state) 
{
    result_t res;

    SDL_Init(SDL_INIT_VIDEO);

    render_state_t *render_state = malloc(sizeof(render_state_t));
    if (!render_state) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "render state allocation failed");
        return RESULT_ERROR_ALLOC;
    }

    render_state->window = SDL_CreateWindow("SPARRY",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            SCREEN_WIDTH,
                                            SCREEN_HEIGHT,
                                            SDL_WINDOW_SHOWN);
    if (!render_state->window) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "render state window creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    LOG(LOG_LEVEL_INFO, "window: %p", render_state->window);
    SDL_RaiseWindow(render_state->window);
    
    render_state->renderer = SDL_CreateRenderer(render_state->window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
    if (!render_state->renderer) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "render state renderer creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    LOG(LOG_LEVEL_INFO, "renderer: %p", render_state->renderer);

    render_state->texture = SDL_CreateTexture(render_state->renderer,
                                              SDL_PIXELFORMAT_BGRA32,
                                              SDL_TEXTUREACCESS_STREAMING,
                                              SCREEN_WIDTH,
                                              SCREEN_HEIGHT);
    if (!render_state->texture) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "render state texture creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    LOG(LOG_LEVEL_INFO, "texture: %p", render_state->texture);

    res = raster_context_init(&render_state->raster_ctx);
    if (res != RESULT_OK) {
        LOG(LOG_LEVEL_ERROR,
            __FILE__, __FUNCTION__, __LINE__,
            "render state raster ctx creation failed"); 
        goto err;
    }

    *out_render_state = render_state;
    return RESULT_OK;

err:
    render_state_destroy(render_state);
    return res;
}

void render_state_destroy(render_state_t *render_state)
{
    if (render_state->texture)    SDL_DestroyTexture(render_state->texture);
    if (render_state->renderer)   SDL_DestroyRenderer(render_state->renderer);
    if (render_state->window)     SDL_DestroyWindow(render_state->window);
    if (render_state->raster_ctx) raster_context_destroy(render_state->raster_ctx);
    if (render_state)             free(render_state);
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

