#include "../image.h"
SDL_Surface* GrayScale(SDL_Surface* image_surface,SDL_Surface* workingZone)
{
    SDL_Surface* res = SDL_CreateRGBSurface(0,image_surface->w,image_surface->h,32,0,0,0,0);
    int width = image_surface->w;
    int height = image_surface->h;
    for(int x = 0; x<width; x++)
    {
	for(int y = 0; y<height; y++)
	{
		Uint32 pixel = getPixel(image_surface, x, y);
		Uint8 r, g, b;
		SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		Uint8 avg = 0.3*r+0.59*g+0.11*b;
		pixel = SDL_MapRGB(image_surface->format, avg, avg, avg);
                if(getPixel(workingZone,x,y))
		    setPixel(res, x, y, pixel);
	}
    }
    return res;
}
