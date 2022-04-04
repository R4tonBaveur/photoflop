#include "image.h"
#ifndef SURFACESTACK_H
#define
typedef struct {
    SDL_Surface* Surface;
    SurfaceStack* next;
} SurfaceStack;
void push(SDL_Surface* Surface,
