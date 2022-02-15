#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "edges.h"
#include "grayscale.h"
#include "noise.h"

void init_sdl();

SDL_Surface* load_image(char *path);

void wait_for_keypressed();

void copy_image (SDL_Surface* src, SDL_Surface* dst);

#endif
