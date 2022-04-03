#include <stdio.h>
#include "image.h"
#include "get_coord.h"
#include "circle_select.h"
#include "sepia.h"
#include "correct.h"
#include "brightness.h"

int main()
{
    size_t x1;
    size_t y1;
    size_t x2 = 0;
    size_t y2 = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* image = loadImage("./images/cat.jpg");
    get_coord(image, &x1, &y1, &x2, &y2);
    //circle_select(image, &x1, &y1, 290);
    SDL_Surface* new = brightness(image, &x1, &y1, &x2, &y2, 2);
    displaySurface(new);

    SDL_FreeSurface(image);
    SDL_FreeSurface(new);
    SDL_Quit();
    return 0;
}
