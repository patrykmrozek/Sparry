#include <math.h>
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

#define NEAR 1.0f
#define FAR 10.0f
#define FOV 90.0f //deg

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
static inline v3 v3_cross(v3 v1, v3 v2)
{
    return (v3){
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x),
    };
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
static inline v3 v3_neg(v3 v)
{
    return (v3){-v.x, -v.y, -v.z};
}

typedef struct v4_s {
    f32 x,y,z,w;
} v4;
typedef struct m4_s {
    v4 v[4];
} m4;
static inline v4 v3_to_v4(v3 v)
{
    return (v4){v.x, v.y, v.z, 1};
}
static inline v4 v4_norm(v4 v)
{
    return (v4){v.x/v.w, v.y/v.w, v.z/v.w, v.w/v.w}; 
}
static inline v3 v4_to_v3(v4 v)
{
    if (v.w != 1) v = v4_norm(v);
    return (v3){v.x, v.y, v.z};
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
        (v4){t.x+v.x, t.y+v.y, t.z+v.z, 1},
    };
    return m4_v4_mul(tm, v);
}
static inline v3 v3_trans(v3 v, v3 t)
{
    v4 ret = v4_trans(v3_to_v4(v), t);
    return v4_to_v3(ret);
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

camera_t g_camera = (camera_t){
    .pos=(v3){0, 1, -1},
    .up=(v3){0,  1,   0},
    .look_at=(v3){0,  0,   0}
};

static inline void to_view_space(v3 *_v)
{
    //first of all, move world - camera
    //for all elements in the world:
    *_v = v3_trans(*_v, v3_neg(g_camera.pos)); //something like this?

    //next, we need to orient so that the up side is up :|
    //cam c is looking at l, dir d is l-c norm(d) is look_at dir
    v3 n = v3_norm(v3_sub(g_camera.look_at, g_camera.pos)); //i guess?
    //this dir represents the new "z" axis (as its perpendicular to our viewing plane / screen)
    
    //since we know up dir also, we can use this and d to find axis orthogonal to both up and d
    v3 u = v3_norm(v3_cross(g_camera.up, n)); //?
    //this is our new x axis in out viewing plane
    
    //we can finally then get our new y axis (up vector) like:
    v3 v = v3_cross(u, n);
    //since its orthogonal to both our new z axis and our new x axis, and so our new y axis
    
    //putting these all together, we can form a sigle change of coord matrix:
    m4 view_mat = (m4){
      (v4){u.x, v.x, n.x, 0},
      (v4){u.y, v.y, n.y, 0},
      (v4){u.z, v.z, n.z, 0},
      (v4){0,   0,    0,   1},
    };
    
    //and then the object can be moved to view space after multiplying by this matrix to it
    *_v = v4_to_v3((m4_v4_mul(view_mat, v3_to_v4(*_v)))); //?
}

static inline void to_ndc(v3 *_v)
{
    f32 width = -2 * NEAR * tan(FOV/2);
    f32 height = width / (SCREEN_WIDTH/SCREEN_HEIGHT);

    m4 proj_mat = (m4){
        (v4){(2*NEAR)/height, 0,               0,                      0},
        (v4){0,               (2*NEAR)/height, 0,                      0},
        (v4){0,               0,               -(FAR+NEAR)/FAR-NEAR,  -1},
        (v4){0,               0,               (-2*FAR*NEAR)/FAR-NEAR, 0},
    };

    *_v = v4_to_v3(v4_norm(m4_v4_mul(proj_mat, v3_to_v4(*_v))));
}

static inline void put_pixel(u32 x, u32 y, u32 z, u32 c)
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
static inline void put_pixel_vec(v3 v, u32 c)
{
    u32 idx = (v.y * SCREEN_WIDTH) + v.x;
    if (idx < 0 || idx >= SCREEN_HEIGHT*SCREEN_WIDTH) {
        return;
    }
    printf("idx: %d\n", idx);
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
            const uint8_t* keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_W]) g_camera.pos.z += 0.1;
            if (keystate[SDL_SCANCODE_S]) g_camera.pos.z -= 0.1;
            if (keystate[SDL_SCANCODE_A]) g_camera.pos.x -= 0.1;
            if (keystate[SDL_SCANCODE_D]) g_camera.pos.x += 0.1;
        }
        /*
        for (int i = 0; i < 100; i++) {
            framebuffer[10000+i] = 0xFFFF0000;
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+i, 5, 0xFF00FF00);
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+100-i, 10, 0xFF0000FF);
            put_pixel_vec((v3){(SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+50, 7}, 0xFFFFFF00);
        }
        */
        v3 point = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10};
        to_view_space(&point);
        to_ndc(&point);
        printf("point: {%f, %f, %f}\n", point.x, point.y, point.z);
        put_pixel_vec(point, 0xFFFF0000);
        
        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
