#include "image.h"
#include <stdlib.h>

SDL_Surface* sepia(SDL_Surface* image_surface, size_t *x1, size_t *y1, size_t *x2, size_t *y2)
{
    //SDL_Surface* res = SDL_CreateRGBSurface(0, image_surface->w, image_surface->h,32,0,0,0,0);
    SDL_Surface* res = image_surface;
    for(size_t x = *x1; x < *x2; x++)
    {
        for(size_t y = *y1; y < *y2; y++)
        {
            Uint32 pixel = getPixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(image_surface->format, r * 0.393 + g * 0.769 + b * 0.189, r * 0.349 + g * 0.686 + b * 0.168, r * 0.272 + g * 0.534 + b * 0.131);
            setPixel(res, x, y, pixel);
        }
    }
    return res;
}
