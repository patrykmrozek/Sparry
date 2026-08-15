#include "render.h"
#include "raster.h"
#include "result.h"

result_t rd_state_init(rd_state_t **state_pp) 
{
    result_t res;

    rd_state_t *state = malloc(sizeof(rd_state_t));
    HANDLE_ERROR_RET(state, RESULT_ERROR_ALLOC, "rd state alloc");

    SDL_Init(SDL_INIT_VIDEO);
    state->window = SDL_CreateWindow("SPARRY",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     SCREEN_WIDTH,
                                     SCREEN_HEIGHT,
                                     SDL_WINDOW_SHOWN);
    HANDLE_ERROR_TAG(state->window, RESULT_ERROR_ALLOC,
                     "rd window create", res, err);
    INFO("window: %p", state->window);
    SDL_RaiseWindow(state->window);
    
    state->renderer = SDL_CreateRenderer(state->window, -1,
                                         SDL_RENDERER_ACCELERATED);
    HANDLE_ERROR_TAG(state->renderer, RESULT_ERROR_ALLOC,
                     "rd state renderer", res, err);
    INFO("renderer: %p", state->renderer);

    state->texture = SDL_CreateTexture(state->renderer,
                                       SDL_PIXELFORMAT_BGRA32,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       SCREEN_WIDTH,
                                       SCREEN_HEIGHT);
    HANDLE_ERROR_TAG(state->texture, RESULT_ERROR_ALLOC,
                     "rd state texture", res, err);
    INFO("texture: %p", state->texture);

    res = rt_ctx_init(&state->rt_ctx); 
    HANDLE_ERROR_TAG(res==RESULT_OK, RESULT_ERROR_ALLOC,
                     "rt_ctx_init", res, err);

    *state_pp = state;
    return RESULT_OK;

err:
    rd_state_destroy(state);
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

