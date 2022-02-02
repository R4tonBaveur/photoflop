#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "image.h"
#include "get_coord.h"

int main()
{
    size_t x;
    size_t y;


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* image = IMG_Load("./images/cat.jpg");

    displaySurface(image);
    //get_coord(image, &x, &y);
    //printf("x = %zu\n y = %zu\n", x, y);

    return 0;
}
