#include "SDL2/SDL_render.h"
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300

typedef uint32_t u32;
typedef float f32;

static u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

typedef struct vec3_s {
    float x,y,z;
} vec3;

typedef struct vec4_s {
    float x,y,z,w;
} vec4;

int main() {
    printf("SPARRY\n");

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(
		"SPARRY",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
    printf("window: %p\n", window);
	SDL_RaiseWindow(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    printf("renderer: %p\n", renderer);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);


    SDL_Event event;
    int game_running = 1;

    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

    framebuffer[100] = 0xFFFF0000;
        framebuffer[5000] = 0xFFFF0000;

        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
