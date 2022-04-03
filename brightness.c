#include "image.h"
#include <stdlib.h>
#include <math.h>

SDL_Surface* brightness(SDL_Surface* image_surface, size_t *x1, size_t *y1, size_t *x2, size_t *y2, int n)
{
    //SDL_Surface* res = SDL_CreateRGBSurface(0, image_surface->w, image_surface->h,32,0,0,0,0);
    for(size_t x = 0; x < image_surface->w; x++)
    {
        for(size_t y = 0; y < image_surface->h; y++)
        {
            Uint32 pixel = getPixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            printf(" r = %d\n", g);
            printf(" _r = %d\n",(int) (255 * pow((double) g/255, n)));
            pixel = SDL_MapRGB(image_surface->format, (int) (255 * pow((double) r/255, n)), (int) (255 * pow((double) g/255, n)), (int) (255 * pow((double) b/255, n)));
            setPixel(image_surface, x, y, pixel);
        }
    }
    return image_surface;
}
