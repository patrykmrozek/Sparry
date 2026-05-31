#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef uint32_t u32;
typedef int i32;
typedef float f32;

static u32 framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static f32 zbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

#define PI 3.14159265359f

#define DEG_TO_RAD(_d) ((_d) * (PI / 180)) 
#define RAD_TO_DEG(_d) ((_d) * (180 / PI))

#define NEAR 1.0f
#define FAR 100000.0f
#define FOV DEG_TO_RAD(90) 

#define M4_ID (m4){{   \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}}}

#define M4_TRANS(_t) (m4){{       \
    {1, 0, 0, _t.x}, \
    {0, 1, 0, _t.y}, \
    {0, 0, 1, _t.z}, \
    {0, 0, 0,  1}}}

#define M4_SCALE(_s) (m4){{  \
    {_s,     0,     0,   0}, \
    {  0,   _s,     0,   0}, \
    {  0,     0,   _s,   0}, \
    {  0,     0,    0,   1}}}

#define M4_SCALEV(_s) (m4){{   \
    {_s.x,     0,     0,   0}, \
    {  0,   _s.y,     0,   0}, \
    {  0,     0,   _s.z,   0}, \
    {  0,     0,     0,    1}}} 

#define M4_ROTX(_a) (m4){{     \
    {1,       0,       0, 0}, \
    {0, cos(_a), -sin(_a), 0}, \
    {0, sin(_a), cos(_a), 0}, \
    {0,       0,       0, 1}}}

#define M4_ROTY(_a) (m4){{     \
    { cos(_a), 0, sin(_a), 0}, \
    {       0, 1,        0, 0}, \
    {-sin(_a), 0,  cos(_a), 0}, \
    {       0, 0,        0, 1}}}

#define M4_ROTZ(_a) (m4){{     \
    {cos(_a), -sin(_a), 0, 0}, \
    {sin(_a),  cos(_a), 0, 0}, \
    {       0,       0, 1, 0}, \
    {       0,       0, 0, 1}}}

typedef struct v3_s {
    f32 x,y,z;
} v3;
static inline v3 v3_init(f32 x, f32 y, f32 z)
{
    return (v3){
        .x=x,
        .y=y,
        .z=z
    };
}
static inline void v3_print(v3 v)
{
    printf("{ %f, %f, %f }\n", v.x, v.y, v.z);
}
static inline v3 v3_fill(f32 f)
{
    return (v3){f, f, f};
}
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
    f32 v[4][4]; //[row][col]
} m4;
static inline v4 v3_to_v4(v3 v)
{
    return (v4){v.x, v.y, v.z, 1};
}
static inline v4 v4_norm(v4 v)
{
    return (v4){v.x/v.w, v.y/v.w, v.z/v.w, v.w/v.w}; 
}
static inline void v4_print(v4 v)
{
    printf("{ %f, %f, %f, %f }\n", v.x, v.y, v.z, v.w);
}
static inline v3 v4_to_v3(v4 v)
{
    if (v.w != 1) v = v4_norm(v);
    return (v3){v.x, v.y, v.z};
}
static inline f32 v4_dot(v4 v1, v4 v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w*v2.w));
}
static inline v4 m4_v4_mul(m4 m, v4 v)
{
    f32 out[4];
    for (int row = 0; row < 4; row++) {
        out[row] = v4_dot((v4){m.v[row][0], m.v[row][1], m.v[row][2], m.v[row][3]}, v);
    }
    return (v4){out[0], out[1], out[2], out[3]};
}
static inline m4 m4_mul(m4 m1, m4 m2)
{
    m4 out;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out.v[i][j] = v4_dot(
                (v4){m1.v[i][0], m1.v[i][1], m1.v[i][2], m1.v[i][3]},
                (v4){m2.v[0][j], m2.v[1][j], m2.v[2][j], m2.v[3][j]});
        }
    }
    return out;
}
static inline void m4_print(m4 m)
{
    for (int row = 0; row < 4; row++) {
        v4_print((v4){m.v[row][0], m.v[row][1], m.v[row][2], m.v[row][3]});
    }
}
static inline v4 v4_trans(v4 v, v3 t)
{
    return m4_v4_mul(M4_TRANS(t), v);
}
static inline v3 v3_trans(v3 v, v3 t)
{
    return v4_to_v3(v4_trans(v3_to_v4(v), t));
}
static inline v4 v4_scale(v4 v, f32 s)
{
    return m4_v4_mul(M4_SCALE(s), v);
}
static inline v4 v4_scalev(v4 v, v3 s)
{
    return m4_v4_mul(M4_SCALEV(s), v);
}

static inline f32 lerp(f32 a, f32 b, f32 t)
{
    return a + (b - a) * t;
}
static inline v3 v3_lerp(v3 a, v3 b, f32 t)
{
    return (v3){
        .x = lerp(a.x, b.x, t),
        .y = lerp(a.y, b.y, t),
        .z = lerp(a.z, b.z, t)
    };
}

typedef struct camera_s {
    v3 pos;
    v3 up;
    v3 look_at;
} camera_t;

camera_t g_camera = (camera_t){
    .pos=(v3){0, 0, -100},
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
    v3 u = v3_norm(v3_cross(n, g_camera.up)); //?
    //this is our new x axis in out viewing plane
    
    //we can finally then get our new y axis (up vector) like:
    v3 v = v3_cross(u, n);
    //since its orthogonal to both our new z axis and our new x axis, and so our new y axis
    
    //putting these all together, we can form a sigle change of coord matrix:
    m4 view_mat = (m4){{
      {u.x, u.y, u.z, 0},
      {v.x, v.y, v.z, 0},
      {n.x, n.y, n.z, 0},
      {0,   0,    0,   1},
    }};
    
    //and then the object can be moved to view space after multiplying by this matrix to it
    *_v = v4_to_v3((m4_v4_mul(view_mat, v3_to_v4(*_v)))); //?
}

static inline void to_ndc(v3 *_v)
{


    f32 aspect = (f32)(SCREEN_WIDTH/(f32)SCREEN_HEIGHT);
    f32 f = 1.0f / tanf(FOV * 0.5);

    m4 proj_mat = (m4){{
        {f/aspect, 0,               0,                        0},
        {0,               f, 0,                        0},
        {0,               0,               -(FAR+NEAR)/(NEAR-FAR),  (-2*FAR*NEAR)/(NEAR-FAR)},
        {0,               0,               -1, 0},
    }};

    *_v = v4_to_v3(v4_norm(m4_v4_mul(proj_mat, v3_to_v4(*_v))));
}

static inline void to_screen(v3 *v)
{
    v->x = (v->x + 1.0f) * 0.5f * SCREEN_WIDTH;
    v->y = (1.0f - v->y) * 0.5f * SCREEN_HEIGHT;
}

static inline v3 *world_to_screen(v3 *v)
{
    printf("ORIGINAL: ");
    v3_print(*v);
    to_view_space(v);
    printf("VIEW: ");
    v3_print(*v);
    if (v->z <= NEAR || v->z >= FAR) return (v3*){0}; //clip
    to_ndc(v);
    printf("NDC: ");
    v3_print(*v);
    to_screen(v);
    printf("SCREEN: ");
    v3_print(*v);

    return v;
}
static inline void put_pixel(i32 x, i32 y, i32 z, u32 c)
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
    v.x = (i32)v.x;
    v.y = (i32)v.y;
    if (v.x < 0 || v.x >= SCREEN_WIDTH || v.y < 0 || v.y >= SCREEN_HEIGHT) return;

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

    m4 m = (m4){{
        {2, 0, 0, 0},
        {0, 2, 0, 0},
        {0, 0, 2, 0},
        {1, 2, 3, 1},
    }};
    m4 res = m4_mul(m, M4_ID);
    m4_print(res);
    printf("\n");
    res = m4_mul(M4_ID, m);
    m4_print(res);

    const uint8_t* keystate = SDL_GetKeyboardState(NULL);
    while (game_running == 1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_running = 0;
            }
            if (keystate[SDL_SCANCODE_W]) g_camera.pos.z += 1.0f;
            if (keystate[SDL_SCANCODE_S]) g_camera.pos.z -= 1.0f;
            if (keystate[SDL_SCANCODE_A]) g_camera.pos.x -= 1.0f;
            if (keystate[SDL_SCANCODE_D]) g_camera.pos.x += 1.0f;
            if (keystate[SDL_SCANCODE_SPACE]) g_camera.pos.y += 1.0f;
            if (keystate[SDL_SCANCODE_LSHIFT]) g_camera.pos.y -= 1.0f;
        }
        /*
        for (int i = 0; i < 100; i++) {
            framebuffer[10000+i] = 0xFFFF0000;
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+i, 5, 0xFF00FF00);
            put_pixel((SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+100-i, 10, 0xFF0000FF);
            put_pixel_vec((v3){(SCREEN_WIDTH/2)+i, (SCREEN_HEIGHT/2)+50, 7}, 0xFFFFFF00);
        }
        */

        //memset(framebuffer, 0, sizeof(framebuffer));

        v3 p1 = {-10, -10, 0};
        v3 p2 = { 10, -10, 0};
        v3 p3 = {-10,  10, 0};
        v3 p4 =  {10,  10, 0};
        v3 ps[4] = {p1, p2, p3, p4};

        for (int i = 0; i < 4; i++) {
            if (world_to_screen(&ps[i])) {
                //printf("P[%d]: ", i);
                printf("\n");
                //v3_print(ps[i]);
                put_pixel_vec(ps[i], 0xFFFFFFFF);
            }
        }
        
        SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
