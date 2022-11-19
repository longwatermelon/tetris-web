#ifndef CUBE_H
#define CUBE_H

#include "util.h"
#include "camera.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

struct Cube
{
    Vec3f pos;
    Vec3f points[8];
    SDL_Color color;
};

struct Cube *cube_alloc(Vec3f pos, SDL_Color col);
void cube_free(struct Cube *cube);

void cube_render(struct Cube *cube, SDL_Renderer *rend, struct Camera *c, bool fill);
void cube_draw_line(struct Cube *cube, SDL_Renderer *rend, struct Camera *c, int i1, int i2);

#endif

