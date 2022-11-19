#include "prog.h"
#include "cube.h"
#include "piece.h"

struct timespec now;

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;

    p->window = w;
    p->rend = r;

    // Board is 10x20
    p->board = calloc(10 * 20 + 1, sizeof(char));

    for (int i = 0; i < 20; ++i)
        strcat(p->board, "..........");

    p->piece = 0;

    p->pieces = 0;
    p->npieces = 0;

    p->borders = 0;
    p->nborders = 0;

    prog_create_borders(p);

    p->camera = camera_alloc((Vec3f){ 0.f, 0.f, 0.f }, (Vec3f){ 0.f, 0.f, 0.f });

    p->camera_follow_piece = false;
    p->camera_rotate = false;
    p->fill = true;

    p->restart = false;

    clock_gettime(CLOCK_MONOTONIC, &p->last_moved);
    clock_gettime(CLOCK_MONOTONIC, &now);

    return p;
}


void prog_free(struct Prog *p)
{
    for (size_t i = 0; i < p->npieces; ++i)
        piece_free(p->pieces[i]);

    free(p->pieces);
    piece_free(p->piece);

    for (size_t i = 0; i < p->nborders; ++i)
        cube_free(p->borders[i]);

    free(p->borders);

    free(p->board);
    camera_free(p->camera);

    free(p);
}


void prog_mainloop(struct Prog *p)
{
    SDL_Event evt;


        prog_handle_events(p, &evt);

        if (p->camera_follow_piece && p->piece)
        {
            p->camera->pos = p->piece->renders[1]->pos;
            p->camera->pos.z = 5.f;
        }

        if (p->camera_rotate)
            prog_rotate_camera(p);

        SDL_RenderClear(p->rend);

        if (!p->piece)
            p->piece = piece_alloc((SDL_Point){ 5, 0 });

        if (util_timediff(&p->last_moved, &now) > 0.35f)
        {
            if (!piece_move(p->piece, p->board, (SDL_Point){ 0, 1 }))
            {
                for (int i = 0; i < 4; ++i)
                    p->piece->renders[i]->color = (SDL_Color){ 255, 255, 255 };

                for (int i = 0; i < 4; ++i)
                    p->board[util_coords_to_index(p->piece->cubes[i], 10)] = '#';

                p->pieces = realloc(p->pieces, sizeof(struct Piece*) * ++p->npieces);
                p->pieces[p->npieces - 1] = p->piece;

                p->piece = piece_alloc((SDL_Point){ 5, 0 });

                prog_detect_line(p);
            }

            clock_gettime(CLOCK_MONOTONIC, &p->last_moved);
        }

        for (size_t i = 0; i < p->nborders; ++i)
            cube_render(p->borders[i], p->rend, p->camera, false);

        for (size_t i = 0; i < p->npieces; ++i)
            piece_render(p->pieces[i], p->rend, p->camera, p->fill);

        if (p->piece)
            piece_render(p->piece, p->rend, p->camera, p->fill);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);

        clock_gettime(CLOCK_MONOTONIC, &now);
}


void prog_handle_events(struct Prog *p, SDL_Event *evt)
{
    while (SDL_PollEvent(evt))
    {
        switch (evt->type)
        {
        case SDL_QUIT:
            p->running = false;
            break;
        case SDL_KEYDOWN:
        {
            switch (evt->key.keysym.sym)
            {
            case SDLK_RIGHT: piece_move(p->piece, p->board, (SDL_Point){ 1, 0 }); break;
            case SDLK_LEFT: piece_move(p->piece, p->board, (SDL_Point){ -1, 0 }); break;
            case SDLK_UP: piece_rotate(p->piece); break;
            case SDLK_SPACE:
                while (piece_move(p->piece, p->board, (SDL_Point){ 0, 1 }))
                    ;
                p->last_moved.tv_sec = 0;
                break;
            case SDLK_DOWN: piece_move(p->piece, p->board, (SDL_Point){ 0, 1 }); break;

            case SDLK_1:
                p->camera->pos = (Vec3f){ 0.f, 0.f, 0.f };
                p->camera->angle = (Vec3f){ 0.f, 0.f, 0.f };
                break;
            case SDLK_2:
                p->camera->pos = (Vec3f){ 10.f, -4.f, 0.f };
                p->camera->angle = (Vec3f){ -1.f, .3f, 0.f };
                break;
            case SDLK_3:
                p->camera->pos = (Vec3f){ -10.f, 0.f, 8.f };
                p->camera->angle = (Vec3f){ 1.f, .1f, 0.f };
                break;
            case SDLK_4:
                p->camera->pos = (Vec3f){ 0.f, 15.f, 0.f };
                p->camera->angle = (Vec3f){ 0.f, -1.f, 0.f };
                break;
            case SDLK_5:
                p->camera->pos = (Vec3f){ 0.f, -15.f, 0.f };
                p->camera->angle = (Vec3f){ 0.f, 1.f, 0.f };
                break;
            case SDLK_6:
                p->camera->pos = (Vec3f){ 0.f, 0.f, 30.f };
                p->camera->angle = (Vec3f){ M_PI, 0.f, 0.f };
                break;

            case SDLK_z: p->camera_follow_piece = !p->camera_follow_piece; break;
            case SDLK_r: p->camera_rotate = !p->camera_rotate; break;
            case SDLK_f: p->fill = !p->fill; break;
            case SDLK_q: p->restart = true; p->running = false; break;
            }
        } break;
        }
    }
}


void prog_create_borders(struct Prog *p)
{
    p->nborders = 10 + 20 * 2;
    p->borders = malloc(sizeof(struct Cube*) * p->nborders);

    for (int i = 0; i < 10; ++i)
        p->borders[i] = cube_alloc((Vec3f){ i - 5, 10, 15 }, (SDL_Color){ 200, 200, 200 });

    for (int i = 0; i < 20; ++i)
        p->borders[i + 10] = cube_alloc((Vec3f){ -6, i - 9, 15 }, (SDL_Color){ 200, 200, 200 });

    for (int i = 0; i < 20; ++i)
        p->borders[i + 10 + 20] = cube_alloc((Vec3f){ 5, i - 9, 15 }, (SDL_Color){ 200, 200, 200 });
}


void prog_detect_line(struct Prog *p)
{
    for (int y = 0; y < 20; ++y)
    {
        bool line = true;

        for (int x = 0; x < 10; ++x)
        {
            if (p->board[y * 10 + x] == '.')
                line = false;
        }

        if (line)
            prog_clear_line(p, y);
    }
}


void prog_clear_line(struct Prog *p, int y)
{
    for (int i = 0; i < 10; ++i)
        p->board[y * 10 + i] = '.';

    for (size_t i = 0; i < p->npieces; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (p->pieces[i]->renders[j])
            {
                if (p->pieces[i]->cubes[j].y == y)
                {
                    cube_free(p->pieces[i]->renders[j]);
                    p->pieces[i]->renders[j] = 0;
                }
                else if (p->pieces[i]->cubes[j].y < y)
                {
                    int index = util_coords_to_index(p->pieces[i]->cubes[j], 10);

                    if (p->board[index] == '#')
                        p->board[util_coords_to_index(p->pieces[i]->cubes[j], 10)] = '.';

                    ++p->pieces[i]->cubes[j].y;
                    ++p->pieces[i]->renders[j]->pos.y;
                    // @ indicates pieces that have been newly moved after the line clear, so that they aren't
                    // cleared by mistake by another moving piece later
                    p->board[util_coords_to_index(p->pieces[i]->cubes[j], 10)] = '@';
                }
            }
        }
    }

    for (int i = 0; i < strlen(p->board); ++i)
        if (p->board[i] == '@')
            p->board[i] = '#';
}


void prog_rotate_camera(struct Prog *p)
{
    float angle = .06f;

    p->camera->angle.z += angle;

    Vec3f center = { 0.f, 0.f, 15.f };

    float dx = p->camera->pos.x - center.x;
    float dz = p->camera->pos.z - center.z;

    p->camera->angle.x -= angle;
    p->camera->pos.x = cosf(angle) * dx - sinf(angle) * dz + center.x;
    p->camera->pos.z = sinf(angle) * dx + cosf(angle) * dz + center.z;
}

