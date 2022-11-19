#ifndef PROG_H
#define PROG_H

#include "camera.h"
#include "piece.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

struct Prog
{
    bool running;

    SDL_Window *window;
    SDL_Renderer *rend;

    char *board;

    struct Piece *piece;
    struct Piece **pieces;
    size_t npieces;

    struct Cube **borders;
    size_t nborders;

    struct Camera *camera;

    bool camera_follow_piece;
    bool camera_rotate;
    bool fill;

    struct timespec last_moved;

    bool restart;
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);
void prog_handle_events(struct Prog *p, SDL_Event *evt);

void prog_create_borders(struct Prog *p);

void prog_detect_line(struct Prog *p);
void prog_clear_line(struct Prog *p, int y);

void prog_rotate_camera(struct Prog *p);

#endif

