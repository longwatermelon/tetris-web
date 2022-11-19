#include "piece.h"


struct Piece *piece_alloc(SDL_Point pos)
{
    struct Piece *p = malloc(sizeof(struct Piece));

    piece_random(p, pos);

    for (int i = 0; i < 4; ++i)
        p->renders[i] = cube_alloc((Vec3f){ p->cubes[i].x - 5, p->cubes[i].y - 10, 15 }, p->color);

    return p;
}


void piece_free(struct Piece *p)
{
    for (int i = 0; i < 4; ++i)
        if (p->renders[i])
            cube_free(p->renders[i]);

    free(p);
}


void piece_render(struct Piece *p, SDL_Renderer *rend, struct Camera *c, bool fill)
{
    for (int i = 0; i < 4; ++i)
        if (p->renders[i])
        {
            cube_render(p->renders[i], rend, c, fill);

            if (p->renders[i]->color.r > p->color.r) p->renders[i]->color.r -= 5;
            else p->renders[i]->color.r = p->color.r;
            if (p->renders[i]->color.g > p->color.g) p->renders[i]->color.g -= 5;
            else p->renders[i]->color.g = p->color.g;
            if (p->renders[i]->color.b > p->color.b) p->renders[i]->color.b -= 5;
            else p->renders[i]->color.b = p->color.b;
        }
}


bool piece_move(struct Piece *p, char *board, SDL_Point vec)
{
    for (int i = 0; i < 4; ++i)
        if (!piece_move_cube(p, board, vec, i))
        {
            SDL_Point invert = { -vec.x, -vec.y };
            for (int j = 0; j < i; ++j)
                piece_move_cube(p, board, invert, j);

            return false;
        }

    return true;
}


bool piece_move_cube(struct Piece *p, char *board, SDL_Point vec, int idx)
{
    SDL_Point prev = p->cubes[idx];

    p->cubes[idx].x += vec.x;
    p->cubes[idx].y += vec.y;

    int index = util_coords_to_index(p->cubes[idx], 10);

    if (index < 0 || index >= strlen(board) || board[index] != '.' ||
        p->cubes[idx].x < 0 || p->cubes[idx].x >= 10)
    {
        p->cubes[idx] = prev;
        return false;
    }

    p->renders[idx]->pos.x += vec.x;
    p->renders[idx]->pos.y += vec.y;

    return true;
}


void piece_random(struct Piece *p, SDL_Point pos)
{
    int i = rand() % 7;

    switch (i)
    {
    case 0: // bar
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x + 1, pos.y },
            { pos.x + 2, pos.y },
            { pos.x + 3, pos.y }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 142, 255, 241 };
    } break;
    case 1: // square
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x + 1, pos.y },
            { pos.x, pos.y + 1 },
            { pos.x + 1, pos.y + 1 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 255, 255, 100 };
    } break;
    case 2: // left squiggle
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x, pos.y + 1 },
            { pos.x + 1, pos.y + 1 },
            { pos.x + 1, pos.y + 2 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 255, 100, 100 };
    } break;
    case 3: // right squiggle
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x, pos.y + 1 },
            { pos.x - 1, pos.y + 1 },
            { pos.x - 1, pos.y + 2 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 102, 255, 102 };
    } break;
    case 4: // left L
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x, pos.y + 1 },
            { pos.x, pos.y + 2 },
            { pos.x + 1, pos.y + 2 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 255, 178, 102 };
    } break;
    case 5: // right L
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x, pos.y + 1 },
            { pos.x, pos.y + 2 },
            { pos.x - 1, pos.y + 2 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 102, 178, 255 };
    } break;
    case 6: // T
    {
        SDL_Point tmp[4] = {
            { pos.x, pos.y },
            { pos.x + 1, pos.y },
            { pos.x + 2, pos.y },
            { pos.x + 1, pos.y + 1 }
        };

        memcpy(p->cubes, tmp, sizeof(p->cubes));
        p->color = (SDL_Color){ 255, 153, 255 };
    } break;
    }
}


void piece_rotate(struct Piece *p)
{
    for (int i = 0; i < 4; ++i)
    {
        SDL_Point center = p->cubes[1];

        int dx = p->cubes[i].x - center.x;
        int dy = p->cubes[i].y - center.y;

        p->cubes[i].x = -dy + center.x;
        p->cubes[i].y = dx + center.y;

        p->renders[i]->pos.x = p->cubes[i].x - 5;
        p->renders[i]->pos.y = p->cubes[i].y - 10;
    }
}

