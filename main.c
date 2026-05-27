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

typedef struct v3_s {
    f32 x,y,z;
} v3;
static inline v3 v3_add(v3 v1, v3 v2)
{
    return (v3){v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}
static inline v3 v3_sub(v3 v1, v3 v2)
{
    return (v3){v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}
static inline v3 v3_scale(v3 v, f32 s)
{
    return (v3){v.x*s, v.y*s, v.z*s};
}
static inline v3 v3_mul(v3 v1, v3 v2)
{
   return (v3){v1.x*v2.x, v1.y*v2.y, v1.z*v2.z}; 
}
static inline v3 v3_div(v3 v1, v3 v2)
{
    return (v3){v1.x/v2.x, v1.y/v2.y, v1.z/v2.z};
}
static inline f32 v3_dot(v3 v1, v3 v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z));
}
static inline f32 v3_len(v3 v)
{
    return (sqrtf(v3_dot(v, v)));
}
static inline v3 v3_norm(v3 v)
{
    f32 l = v3_len(v);
    return (v3)v3_div(v, (v3){l, l, l});
}

typedef struct v4_s {
    f32 x,y,z,w;
} v4;
typedef struct m4_s {
    v4 v[4];
} m4;
static inline v4 v4_norm(v4 v)
{
    return (v4){v.x/v.w, v.y/v.w, v.z/v.w, v.w/v.w}; 
}
static inline v4 m4_v4_mul(m4 m, v4 v)
{
    return (v4){
        v3_dot((v3){v.x, v.y, v.z}, (v3){m.v[0].x, m.v[1].x, m.v[2].x}) + m.v[3].x,
        v3_dot((v3){v.x, v.y, v.z}, (v3){m.v[0].y, m.v[1].y, m.v[2].y}) + m.v[3].y,
        v3_dot((v3){v.x, v.y, v.z}, (v3){m.v[0].z, m.v[1].z, m.v[2].z}) + m.v[3].z,
        1,
    };
}
static inline v4 v4_trans(v4 v, v3 t)
{
    m4 tm = (m4){
        (v4){1,       0,       0,       0},
        (v4){0,       1,       0,       0},
        (v4){0,       0,       1,       0},
        (v4){t.x*v.x, t.y*v.y, t.z*v.z, 1},
    };
    return m4_v4_mul(tm, v);
}
static inline v4 v4_scale(v4 v, f32 s)
{
    m4 sm = (m4){
        (v4){v.x*s, 0,     0,     0},
        (v4){0,     v.y*s, 0,     0},
        (v4){0,     0,     v.z*s, 0},
        (v4){0,     0,     0,     1},
    };
    return m4_v4_mul(sm, v);
}

typedef struct camera_s {
    v3 pos;
    v3 up;
    v3 look_at;
} camera_t;

static inline void put_pixel(u32 x, u32 y, u32 z, u32 c)
{
    u32 idx = (y * SCREEN_WIDTH) + x;
    if (!zbuffer[idx] || zbuffer[idx]> z) {
        zbuffer[idx] = z;
        framebuffer[idx] = c;
    }
}

static inline void put_pixel_vec(v3 v, u32 c)
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

    v4 v = (v4){1, 1, 1, 1};
    m4 m = (m4){
        (v4){2, 0, 0, 0},
        (v4){0, 2, 0, 0},
        (v4){0, 0, 2, 0},
        (v4){1, 2, 3, 1},
    };
    v4 mv = m4_v4_mul(m, v);
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
            put_pixel_vec((v3){(SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+50, 7}, 0xFFFFFF00);
        }
        
        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
