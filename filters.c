#include "image.h"
#include "grayscale.h"
#include "bicolor.h"
#include "expand.h"
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
    //displaySurface(expand(loadImage("image/cat.jpg"),10));
    SDL_FreeSurface(image);
    return 0;
}
