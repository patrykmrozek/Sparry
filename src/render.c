#include "render.h"
#include "raster.h"
#include "result.h"

result_t 
render_state_init(render_state_t **state_pp) 
{
    result_t res;

    render_state_t *state = malloc(sizeof(render_state_t));
    HANDLE_ERROR_RET(!state, RESULT_ERROR_ALLOC, "rd state alloc");

    SDL_Init(SDL_INIT_VIDEO);
    state->window = SDL_CreateWindow("SPARRY",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     SCREEN_WIDTH,
                                     SCREEN_HEIGHT,
                                     SDL_WINDOW_SHOWN);
    HANDLE_ERROR_TAG(!state->window, RESULT_ERROR_ALLOC,
                     "rd window create", res, err);
    INFO("window: %p", state->window);
    SDL_RaiseWindow(state->window);
    
    state->renderer = SDL_CreateRenderer(state->window, -1,
                                         SDL_RENDERER_ACCELERATED);
    HANDLE_ERROR_TAG(!state->renderer, RESULT_ERROR_ALLOC,
                     "rd state renderer", res, err);
    INFO("renderer: %p", state->renderer);

    state->texture = SDL_CreateTexture(state->renderer,
                                       SDL_PIXELFORMAT_BGRA32,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       SCREEN_WIDTH,
                                       SCREEN_HEIGHT);
    HANDLE_ERROR_TAG(!state->texture, RESULT_ERROR_ALLOC,
                     "rd state texture", res, err);
    INFO("texture: %p", state->texture);
    SDL_SetTextureBlendMode(state->texture, SDL_BLENDMODE_BLEND);

    res = raster_ctx_init(&state->raster_ctx); 
    HANDLE_ERROR_TAG(res!=RESULT_OK, RESULT_ERROR_ALLOC,
                     "raster_ctx_init", res, err);

    *state_pp = state;
    return RESULT_OK;

err:
    render_state_destroy(state);
    return res;
}

void 
render_state_destroy(render_state_t *render_state)
{
    if (render_state->texture)    SDL_DestroyTexture(render_state->texture);
    if (render_state->renderer)   SDL_DestroyRenderer(render_state->renderer);
    if (render_state->window)     SDL_DestroyWindow(render_state->window);
    if (render_state->raster_ctx)     raster_ctx_destroy(render_state->raster_ctx);
    if (render_state)             free(render_state);
}


void 
render_frame_begin(render_state_t *state)
{
   raster_ctx_clear(state->raster_ctx); 
}

void 
render_frame_end(render_state_t *state)
{
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    SDL_UpdateTexture(state->texture,
                      NULL, 
                      state->raster_ctx->fbuffer, 
                      SCREEN_WIDTH * sizeof(u32));

    SDL_RenderCopy(state->renderer,
                   state->texture,
                   NULL,
                   NULL);

    SDL_RenderPresent(state->renderer);
}

