#include "transform.h"

void to_view_space(v3 *_v)
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
      {u.x, u.y, u.z, -v3_dot(u, g_camera.pos)},
      {v.x, v.y, v.z, -v3_dot(v, g_camera.pos)},
      {n.x, n.y, n.z, -v3_dot(n, g_camera.pos)},
      {0,   0,    0,   1},
    }};
    
    //and then the object can be moved to view space after multiplying by this matrix to it
    *_v = v4_to_v3((m4_v4_mul(view_mat, v3_to_v4(*_v)))); //?
}

void to_ndc(v3 *_v)
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

void to_screen(v3 *v)
{
    v->x = (v->x + 1.0f) * 0.5f * SCREEN_WIDTH;
    v->y = (1.0f - v->y) * 0.5f * SCREEN_HEIGHT;
}

v3 *world_to_screen(v3 *v)
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
