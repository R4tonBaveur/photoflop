#include "image.h"
#include <stdlib.h>

size_t abso(size_t a)
{
    if(a < 0)
        return -a;
    return a;
}

SDL_Surface* correct(SDL_Surface* image_surface, size_t *x1, size_t *y1)
{
    //SDL_Surface* res = SDL_CreateRGBSurface(0, image_surface->w, image_surface->h,32,0,0,0,0);
    SDL_Surface* res = image_surface;
    size_t number_pixel = 0;
    int avg_r = 0;
    int avg_g = 0;
    int avg_b = 0;
    for(size_t x = *x1 - 10; x < *x1 + 10; x++)
    {
        for(size_t y = *y1 - 10; y < *y1 + 10; y++)
        {
            if(abso(x - *x1) > 5 && abso(y - *y1) > 5)
            {
                number_pixel++;
                Uint32 pixel = getPixel(image_surface, x, y);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                avg_r += r;
                avg_g += g;
                avg_b += b;
            }
        }
    }
    avg_r = avg_r / number_pixel;
    avg_g = avg_g / number_pixel;
    avg_b = avg_b / number_pixel;

    for(size_t x = *x1 - 5; x <= *x1 + 5; x++)
    {
        for(size_t y = *y1 - 5; y < *y1 + 5; y++)
        {
            Uint32 pixel2 = getPixel(image_surface, x, y);
            Uint8 r2, g2, b2;
            SDL_GetRGB(pixel2, image_surface->format, &r2, &g2, &b2);
            pixel2 = SDL_MapRGB(image_surface->format, avg_r, avg_g, avg_b);
            setPixel(res, x, y, pixel2);
        }
    }
    return res;
}
