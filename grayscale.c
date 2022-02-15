#include "image.h"
#include <stdlib.h>

void grayscale(SDL_Surface* image_surface, size_t *x1, size_t *y1, size_t *x2, size_t *y2)
{

    for(unsigned x = *x1; x < *x2; x++)
    {
        for(unsigned y = *y1; y < *y2; y++)
        {
            Uint32 pixel = getPixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            Uint8 avg = 0.3*r+0.59*g+0.11*b;
            pixel = SDL_MapRGB(image_surface->format, avg, avg, avg);
            setPixel(image_surface, x, y, pixel);
        }
    }
}
