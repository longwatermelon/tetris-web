#include "cube.h"
#include "render.h"


struct Cube *cube_alloc(Vec3f pos, SDL_Color col)
{
    struct Cube *cube = malloc(sizeof(struct Cube));
    cube->pos = pos;
    cube->color = col;

    const static Vec3f tmp[8] = {
        { 0.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 1.f },
        { 0.f, 0.f, 1.f },

        { 0.f, 1.f, 0.f },
        { 1.f, 1.f, 0.f },
        { 1.f, 1.f, 1.f },
        { 0.f, 1.f, 1.f }
    };

    memcpy(cube->points, tmp, sizeof(cube->points));

    return cube;
}


void cube_free(struct Cube *cube)
{
    free(cube);
}


void cube_render(struct Cube *cube, SDL_Renderer *rend, struct Camera *c, bool fill)
{
    SDL_SetRenderDrawColor(rend, cube->color.r, cube->color.g, cube->color.b, 255);

    for (int i = 1; i < 4; ++i)
        cube_draw_line(cube, rend, c, i, i - 1);

    cube_draw_line(cube, rend, c, 3, 0);

    for (int i = 5; i < 8; ++i)
        cube_draw_line(cube, rend, c, i, i - 1);

    cube_draw_line(cube, rend, c, 7, 4);

    for (int i = 0; i < 4; ++i)
        cube_draw_line(cube, rend, c, i, i + 4);

    if (fill)
    {
        cube_draw_line(cube, rend, c, 4, 1);
        cube_draw_line(cube, rend, c, 3, 7);

        cube_draw_line(cube, rend, c, 7, 5);
        cube_draw_line(cube, rend, c, 3, 1);

        cube_draw_line(cube, rend, c, 5, 2);
        cube_draw_line(cube, rend, c, 4, 3);
    }
}


void cube_draw_line(struct Cube *cube, SDL_Renderer *rend, struct Camera *c, int i1, int i2)
{
    Vec3f adjusted_i1 = {
        cube->pos.x + cube->points[i1].x,
        cube->pos.y + cube->points[i1].y,
        cube->pos.z + cube->points[i1].z
    };

    Vec3f adjusted_i2 = {
        cube->pos.x + cube->points[i2].x,
        cube->pos.y + cube->points[i2].y,
        cube->pos.z + cube->points[i2].z
    };

    SDL_Point p1 = render_center_and_scale(render_project_point(adjusted_i1, c));
    SDL_Point p2 = render_center_and_scale(render_project_point(adjusted_i2, c));

    SDL_RenderDrawLine(rend, p1.x, p1.y, p2.x, p2.y);
}

