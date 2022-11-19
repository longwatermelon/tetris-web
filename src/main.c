#include "prog.h"
#include <time.h>
#include <emscripten.h>

void mainloop(void *arg)
{
    struct Prog *p = arg;
    prog_mainloop(p);

    if (!p->running)
        emscripten_cancel_main_loop();
}

int main(int argc, char **argv)
{
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("3d Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while (true)
    {
        struct Prog *p = prog_alloc(window, rend);
        emscripten_set_main_loop_arg(mainloop, p, -1, 1);

        if (!p->restart)
        {
            prog_free(p);
            break;
        }

        prog_free(p);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

