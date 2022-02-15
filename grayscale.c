//#include "grayscale.h"
#include "image.h"

void turn_gray(SDL_Surface* image_surface)
{
    unsigned width = image_surface->w;
    unsigned height = image_surface->h;


    for(unsigned x = 0; x<width; x++)
    {
	for(unsigned y = 0; y<height; y++)
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
