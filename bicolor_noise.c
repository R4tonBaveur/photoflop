#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "display.h"
#include "pixel_operations.h"


void dilate(SDL_Surface* img)
{
    unsigned width = img->w;
    unsigned height = img->h;

    SDL_Surface* tmp= SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_BlitSurface(img, NULL, tmp, NULL);

    Uint32 white_pixel =SDL_MapRGB(img->format, 255, 255, 255);

    for(unsigned x = 1; x<width-1; x++)
    {
	    for(unsigned y = 1; y<height-1; y++)
	    {
    	    Uint32 pixel = get_pixel(tmp, x, y);
	        Uint8 color, null, nul;
    	    SDL_GetRGB(pixel, img->format, &color, &null, &nul);

                if(color!=(Uint8) 0)
                {
                    for(int i = -1; i<=1; i+=2)
                    {
                        put_pixel(img, x+i, y, white_pixel);
                        put_pixel(img, x, y+i, white_pixel);
                    }
                }

        }
    }

    SDL_FreeSurface(tmp);
}


void erode(SDL_Surface* img)
{
    unsigned width = img->w;
    unsigned height = img->h;

    SDL_Surface* tmp= SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_BlitSurface(img, NULL, tmp, NULL);

    Uint32 black_pixel = SDL_MapRGB(img->format, 0, 0, 0);

    for(unsigned x = 1; x<width-1; x++)
    {
	    for(unsigned y = 1; y<height-1; y++)
	    {
    	    Uint32 pixel = get_pixel(tmp, x, y);
	        Uint8 color, null, nul;
    	    SDL_GetRGB(pixel, img->format, &color, &null, &nul);

            if(color==(Uint8) 0)
            {
                for(int i = -1; i<=1; i+=2)
                {
                    put_pixel(img, x+i, y, black_pixel);
                    put_pixel(img, x, y+i, black_pixel);
                }
            }

        }
    }

    SDL_FreeSurface(tmp);
}
