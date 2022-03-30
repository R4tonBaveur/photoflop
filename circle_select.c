#include "image.h"
#include <stdlib.h>

void circle_select(SDL_Surface* image_surface, size_t *x1, size_t *y1, size_t radius)
{

    for(unsigned x = *x1 - radius; x < *x1 + radius; x++)
    {
        for(unsigned y = *y1 - radius; y < *y1 + radius; y++)
        {
            if((x - *x1) * (x - *x1) + (y - *y1) * (y - *y1) <= radius)
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
}
