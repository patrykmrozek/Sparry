#include "render.h"
#include "raster.h"
#include "result.h"

result_t rd_state_init(rd_state_t **state_pp) 
{
    result_t res;

    SDL_Init(SDL_INIT_VIDEO);

    rd_state_t *rd_state = malloc(sizeof(rd_state_t));
    if (!rd_state) {
        ERROR("rd state allocation failed");
        return RESULT_ERROR_ALLOC;
    }

    rd_state->window = SDL_CreateWindow("SPARRY",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!rd_state->window) {
        ERROR("rd state window creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    INFO("window: %p", rd_state->window);
    SDL_RaiseWindow(rd_state->window);
    
    rd_state->renderer = SDL_CreateRenderer(rd_state->window,
                                            -1,
                                            SDL_RENDERER_ACCELERATED);
if (!rd_state->renderer) {
        ERROR("rd state renderer creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    INFO("renderer: %p", rd_state->renderer);

    rd_state->texture = SDL_CreateTexture(rd_state->renderer,
                                          SDL_PIXELFORMAT_BGRA32,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT);
    if (!rd_state->texture) {
        ERROR("rd state texture creation failed");
        res = RESULT_ERROR_ALLOC;
        goto err;
    }
    INFO("texture: %p", rd_state->texture);

    res = rt_ctx_init(&rd_state->rt_ctx);
    if (res != RESULT_OK) {
        ERROR("rd state rt ctx creation failed"); 
        goto err;
    }

    *state_pp = rd_state;
    return RESULT_OK;

err:
    rd_state_destroy(rd_state);
    return res;
}

void rd_state_destroy(rd_state_t *rd_state)
{
    if (rd_state->texture)    SDL_DestroyTexture(rd_state->texture);
    if (rd_state->renderer)   SDL_DestroyRenderer(rd_state->renderer);
    if (rd_state->window)     SDL_DestroyWindow(rd_state->window);
    if (rd_state->rt_ctx)     rt_ctx_destroy(rd_state->rt_ctx);
    if (rd_state)             free(rd_state);
}


void rd_frame_begin(rd_state_t *rd_state)
{
   rt_ctx_clear(rd_state->rt_ctx); 
}

void rd_frame_end(rd_state_t *state)
{
    SDL_UpdateTexture(state->texture,
                      NULL, 
                      state->rt_ctx->fbuffer, 
                      SCREEN_WIDTH * sizeof(u32));

    SDL_RenderCopy(state->renderer,
                   state->texture,
                   NULL,
                   NULL);

    SDL_RenderPresent(state->renderer);
}

