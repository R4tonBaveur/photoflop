#include <SDL2/SDL.h>
#ifndef PHOTOFLOP_IMAGE_H
#define PHOTOFLOP_IMAGE_H
SDL_Surface* turn(SDL_Surface* image,double angle);
void displaySurface(SDL_Surface* image);
Uint32 getPixel(SDL_Surface *image,uint16_t x,uint16_t y);
void setPixel(SDL_Surface *image,uint16_t x,uint16_t y,Uint32 color);
Uint32 RGBToUint32(SDL_Surface *image,uint8_t R,uint8_t G,uint8_t B);
SDL_Surface* loadImage(char* file);
void saveImage(SDL_Surface* image,char* file);
#endif
