#include "transform.h"

v3 to_view_space(v3 v)
{
    v3 view_x, view_y, view_z;
    m4 view_mat;
    v3 ret;

    view_z = v3_norm(v3_sub(g_camera.pos, g_camera.look_at)); // any objects in front have neg z
    view_x = v3_norm(v3_cross(view_z, g_camera.up)); // perp of z and up (right) 
    view_y = v3_norm(v3_cross(view_x, view_z)); // perp of x and z (y)
    
    // rot world into camera coords
    // translate world relative to camera
    view_mat = (m4){{
      {view_x.x, view_x.y, view_x.z, -v3_dot(view_x, g_camera.pos)},
      {view_y.x, view_y.y, view_y.z, -v3_dot(view_y, g_camera.pos)},
      {view_z.x, view_z.y, view_z.z, -v3_dot(view_z, g_camera.pos)},
      {0,   0,    0,   1},
    }};
    
    //and then the object can be moved to view space after multiplying by this matrix to it
    ret = v4_to_v3((m4_v4_mul(view_mat, v3_to_v4(v)))); //?

    return ret;
}

v3 to_ndc(v3 v) //projection
{
    f32 aspect, f;
    m4 proj_mat;
    v3 ret;

    aspect = (f32)(SCREEN_WIDTH/(f32)SCREEN_HEIGHT);
    f = 1.0f / tanf(FOV * 0.5);

    proj_mat = (m4){{
        {f/aspect, 0, 0, 0},
        {0, f, 0, 0},
        {0, 0, -(FAR+NEAR)/(NEAR-FAR), (-2*FAR*NEAR)/(NEAR-FAR)},
        {0, 0, -1, 0},
    }};

    ret = v4_to_v3(v4_norm(m4_v4_mul(proj_mat, v3_to_v4(v))));

    return ret;
}

v3 to_screen(v3 v)
{
    v3 ret;
    ret.x = (v.x + 1.0f) * 0.5f * SCREEN_WIDTH;
    ret.y = (1.0f - v.y) * 0.5f * SCREEN_HEIGHT;
    ret.z = v.z;

    return ret;
}

bool world_to_screen(v3 world, v3 *ret)
{
    v3 view, ndc, screen;

    printf("ORIGINAL: ");
    v3_print(world);
    view = to_view_space(world);
    printf("VIEW: ");
    v3_print(view);
    if (view.z > -NEAR || view.z < -FAR) return false; //clip
    ndc = to_ndc(view);
    printf("NDC: ");
    v3_print(ndc);
    screen = to_screen(ndc);
    printf("SCREEN: ");
    v3_print(screen);

    *ret = screen;
    return true;
}
