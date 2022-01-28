#include "image.h"
#include <SDL2/SDL.h>
#include <stdio.h>
int main(){
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Error SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Surface* cat = loadImage("images/cat.jpg");
    if(cat){
        displaySurface(cat);
    } else {
        printf("Something went wrong while loading the image");
    }
    SDL_FreeSurface(cat);
    return 0;
}
