#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef uint32_t u32;
typedef float f32;

static u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

typedef struct vec3_s {
    f32 x,y,z;
} vec3_t;

typedef struct vec4_s {
    f32 x,y,z,w;
} vec4_t;

typedef struct mat4_s {
    vec4_t v[4];
} mat4_t;

static inline vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    return (vec3_t){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}
static inline vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
    return (vec3_t){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}
static inline vec3_t vec3_mul(vec3_t v1, vec3_t v2)
{
   return (vec3_t){v1.x*v2.x, v1.y*v2.y, v1.z*v2.z}; 
}
static inline vec3_t vec3_div(vec3_t v1, vec3_t v2)
{
    return (vec3_t){v1.x/v2.x, v1.y/v2.y, v1.z/v2.z};
}
static inline f32 vec3_dot(vec3_t v1, vec3_t v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z));
}
static inline f32 vec3_length(vec3_t v)
{
    return (sqrtf(vec3_dot(v, v)));
}
static inline vec3_t vec3_normalize(vec3_t v)
{
    f32 len = vec3_length(v);
    return (vec3_t)vec3_div(v, (vec3_t){len, len, len});
}
static inline vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
    return (vec4_t){
        vec3_dot((vec3_t){v.x, v.y, v.z}, (vec3_t){m.v[0].x, m.v[1].x, m.v[2].x}) + m.v[3].x,
        vec3_dot((vec3_t){v.x, v.y, v.z}, (vec3_t){m.v[0].y, m.v[1].y, m.v[2].y}) + m.v[3].y,
        vec3_dot((vec3_t){v.x, v.y, v.z}, (vec3_t){m.v[0].z, m.v[1].z, m.v[2].z}) + m.v[3].z,
        1,
    };
}

static inline void put_pixel(u32 x, u32 y, u32 z, u32 c)
{
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (!zbuffer[idx] || zbuffer[idx]> z) {
        zbuffer[idx] = z;
        framebuffer[idx] = c;
    }
}

static inline void put_pixel_vec(vec3_t v, u32 c)
{
    u32 idx = (v.y * SCREEN_WIDTH) + v.x;
    if (!zbuffer[idx] || zbuffer[idx]> v.z) {
        zbuffer[idx] = v.z;
        framebuffer[idx] = c;
    }
}

int main() 
{
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
    bool game_running = 1;

    vec4_t v = (vec4_t){1, 1, 1, 1};
    mat4_t m = (mat4_t){
        (vec4_t){2, 0, 0, 0},
        (vec4_t){0, 2, 0, 0},
        (vec4_t){0, 0, 2, 0},
        (vec4_t){1, 2, 3, 1},
    };
    vec4_t mv = mat4_mul_vec4(m, v);
    printf("mv: {%f,%f,%f}\n", mv.x, mv.y, mv.z);

    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
        }

        for (int i = 0; i < 100; i++) {
            framebuffer[10000+i] = 0xFFFF0000;
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+i, 5, 0xFF00FF00);
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+100-i, 10, 0xFF0000FF);
            put_pixel_vec((vec3_t){(SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+50, 7}, 0xFFFFFF00);
        }
        
        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
