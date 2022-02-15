#include <stdlib.h>
#include <stdio.h>
#include "grayscale.h"
#include "image.h"

void get_coord(SDL_Surface* image, size_t *x1, size_t *y1, size_t *x2, size_t *y2)
{
    static int counter = 0;
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
            counter += 1;
            if(counter % 2 == 1)
            {
                *x1 = event.motion.x;
                *y1 = event.motion.y;
            }
            else if (counter % 2 == 0)
            {
                *x2 = event.motion.x;
                *y2 = event.motion.y;
                grayscale(image, x1, y1, x2, y2);
            }
            printf("---------------------\n");
            printf("x1 = %zu\n", *x1);
            printf("y1 = %zu\n", *y1);
            printf("x2 = %zu\n", *x2);
            printf("y2 = %zu\n", *y2);
            break;
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
