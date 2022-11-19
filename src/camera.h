#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

struct Camera
{
    Vec3f pos;
    Vec3f angle;
};

struct Camera *camera_alloc(Vec3f pos, Vec3f angle);
void camera_free(struct Camera *c);

#endif

