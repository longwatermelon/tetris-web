#include "render.h"


SDL_FPoint render_project_point(Vec3f p, struct Camera *c)
{
    SDL_FPoint proj;

    p.x -= c->pos.x;
    p.y -= c->pos.y;
    p.z -= c->pos.z;

    float rotx[3][3] = {
        { 1, 0, 0 },
        { 0, cosf(c->angle.y), sinf(c->angle.y) },
        { 0, -sinf(c->angle.y), cosf(c->angle.y) }
    };

    float roty[3][3] = {
        { cosf(c->angle.x), 0, sinf(c->angle.x) },
        { 0, 1, 0 },
        { -sinf(c->angle.x), 0, cosf(c->angle.x) }
    };

    float rotz[3][3] = {
        { cosf(c->angle.z), sinf(c->angle.z), 0 },
        { -sinf(c->angle.z), cosf(c->angle.z), 0 },
        { 0, 0, 1 }
    };

    p = util_matrix_mul(roty, p);
    p = util_matrix_mul(rotx, p);
    p = util_matrix_mul(rotz, p);

    if (p.z != 0.f)
    {
        proj.x = p.x / p.z;
        proj.y = p.y / p.z;
    }

    return proj;
}


SDL_Point render_center_and_scale(SDL_FPoint p)
{
    return (SDL_Point){
        (p.x + 1.f) * 400.f,
        (p.y + 1.f) * 400.f
    };
}

