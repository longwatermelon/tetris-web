#ifndef RENDER_H
#define RENDER_H

#include "util.h"
#include "camera.h"
#include <SDL2/SDL.h>

SDL_FPoint render_project_point(Vec3f p, struct Camera *c);
SDL_Point render_center_and_scale(SDL_FPoint p);

#endif

