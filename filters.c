#include "image.h"
#include "grayscale.h"
int main(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    SDL_Surface* image = loadImage("images/cat.jpg");
    turn_gray(image);
    displaySurface(image);
    SDL_FreeSurface(image);
    return 0;
}
