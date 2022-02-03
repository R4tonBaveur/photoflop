#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "get_coord.h"

void get_coord(SDL_Surface* image, size_t *x, size_t *y)
{

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Photoflop",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
        image);

    int quit = 0;
    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_MOUSEBUTTONDOWN:
            *x = event.motion.x;
            *y = event.motion.y;
            printf("x = %zu\n", *x);
            printf("y = %zu\n", *y);
            break;
        }

        //SDL_Rect dstrect = { x, y, 64, 64 };

       // SDL_RenderClear(renderer);
       // SDL_RenderCopy(renderer, texture, NULL, &dstrect);
       // SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
