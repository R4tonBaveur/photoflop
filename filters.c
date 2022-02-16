#include "image.h"
#include "grayscale.h"
#include "bicolor.h"
#include "dye.h"
int main(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Surface* image = loadImage("images/cat.jpg");
    turn_gray(image);
    displaySurface(image);
    image = loadImage("images/cat.jpg");
    bicolor(image,10);
    displaySurface(image);
    image = loadImage("images/cat.jpg");
    dye(image,1,1,1);
    displaySurface(image);
    SDL_FreeSurface(image);
    return 0;
}
