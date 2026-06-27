#include "transform.h"

v3 to_view_space(v3 v)
{
    v3 view_cam_trans;
    v3 view_x, view_y, view_z;
    m4 view_mat;
    v3 ret;

    //first of all, move world - camera
    //for all elements in the world:
    view_cam_trans = v3_trans(v, v3_neg(g_camera.pos)); //something like this?

    //next, we need to orient so that the up side is up :|
    //cam c is looking at l, dir d is l-c norm(d) is look_at dir
    view_z = v3_norm(v3_sub(g_camera.look_at, g_camera.pos)); //i guess?
    //this dir represents the new "z" axis (as its perpendicular to our viewing plane / screen)
    
    //since we know up dir also, we can use this and d to find axis orthogonal to both up and d
    view_x = v3_norm(v3_cross(view_z, g_camera.up)); //?
    //this is our new x axis in out viewing plane
    
    //we can finally then get our new y axis (up vector) like:
    view_y = v3_cross(view_x, view_z);
    //since its orthogonal to both our new z axis and our new x axis, and so our new y axis
    
    //putting these all together, we can form a sigle change of coord matrix:
    view_mat = (m4){{
      {view_x.x, view_x.y, view_x.z, -v3_dot(view_x, g_camera.pos)},
      {view_y.x, view_y.y, view_y.z, -v3_dot(view_y, g_camera.pos)},
      {view_z.x, view_z.y, view_z.z, -v3_dot(view_z, g_camera.pos)},
      {0,   0,    0,   1},
    }};
    
    //and then the object can be moved to view space after multiplying by this matrix to it
    ret = v4_to_v3((m4_v4_mul(view_mat, v3_to_v4(view_cam_trans)))); //?

    return ret;
}

v3 to_ndc(v3 v)
{
    f32 aspect, f;
    m4 proj_mat;
    v3 ret;

    ret = v;
    
    aspect = (f32)(SCREEN_WIDTH/(f32)SCREEN_HEIGHT);
    f = 1.0f / tanf(FOV * 0.5);

    proj_mat = (m4){{
        {f/aspect, 0, 0, 0},
        {0, f, 0, 0},
        {0, 0, -(FAR+NEAR)/(NEAR-FAR), (-2*FAR*NEAR)/(NEAR-FAR)},
        {0, 0, -1, 0},
    }};

    ret = v4_to_v3(v4_norm(m4_v4_mul(proj_mat, v3_to_v4(ret))));

    return ret;
}

v3 to_screen(v3 v)
{
    v3 ret;
    ret.x = (v.x + 1.0f) * 0.5f * SCREEN_WIDTH;
    ret.y = (1.0f - v.y) * 0.5f * SCREEN_HEIGHT;

    return ret;
}

v3 world_to_screen(v3 v)
{
    v3 ret;
    ret = v;

    printf("ORIGINAL: ");
    v3_print(ret);
    ret = to_view_space(ret);
    printf("VIEW: ");
    v3_print(ret);
    if (ret.z <= NEAR || ret.z >= FAR) return (v3){0}; //clip
    ret = to_ndc(ret);
    printf("NDC: ");
    v3_print(ret);
    ret = to_screen(ret);
    printf("SCREEN: ");
    v3_print(ret);

    return ret;
}
