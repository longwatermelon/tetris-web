#include "camera.h"


struct Camera *camera_alloc(Vec3f pos, Vec3f angle)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    c->pos = pos;
    c->angle = angle;

    return c;
}


void camera_free(struct Camera *c)
{
    free(c);
}

