#include <SDL2/SDL_events.h>
#include <stddef.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "get_coord.h"

void get_coord(SDL_Surface* image, size_t *x, size_t *y)
{
    SDL_Event event;
    int quit = 0;

    while(!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                *x = event.motion.x;
                *y = event.motion.y;
                break;
        }
    }
}
