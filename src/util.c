#include "util.h"


float util_timediff(struct timespec *t1, struct timespec *t2)
{
    return (t2->tv_sec - t1->tv_sec) + (t2->tv_nsec - t1->tv_nsec) / 1e9;
}


int util_coords_to_index(SDL_Point coords, int w)
{
    return coords.y * w + coords.x;
}


Vec3f util_matrix_mul(float mat[3][3], Vec3f p)
{
    Vec3f ret;

    ret.x = p.x * mat[0][0] + p.y * mat[1][0] + p.z * mat[2][0];
    ret.y = p.x * mat[0][1] + p.y * mat[1][1] + p.z * mat[2][1];
    ret.z = p.x * mat[0][2] + p.y * mat[1][2] + p.z * mat[2][2];

    return ret;
}

