#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "image.h"
#include "get_coord.h"

int main()
{
    size_t x1;
    size_t y1;
    size_t x2 = 0;
    size_t y2 = 0;


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* image = IMG_Load("./images/cat.jpg");
    get_coord(image, &x1, &y1, &x2, &y2);
    SDL_FreeSurface(image);
    return 0;
}
