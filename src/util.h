#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <SDL2/SDL.h>

typedef struct
{
    float x, y, z;
} Vec3f;

float util_timediff(struct timespec *t1, struct timespec *t2);

int util_coords_to_index(SDL_Point coords, int w);

Vec3f util_matrix_mul(float mat[3][3], Vec3f p);

#endif

