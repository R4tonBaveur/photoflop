#include "image.h"

int mean(SDL_Surface* img, unsigned x, unsigned y, unsigned size)
{
    int sum = 0;
    int num = 0;

    unsigned width = img->w;
    unsigned height = img->h;
    
    unsigned init_x = 0;
    unsigned init_y = 0;

    if(x>=size)
	init_x = x-size;
    if(y>=size)
	init_y = y-size;

    for(unsigned i = init_x; i< size+x && i<width; i++)
    {
    	for(unsigned j = init_y; j<y+size && j<height; j++, num++)
	{	
   	    Uint32 pixel = getPixel(img, i, j);
    	    Uint8 color, null, nul;
    	    SDL_GetRGB(pixel, img->format, &color, &null, &nul);

	    sum+= (int) color;
	}
    }
    return sum/num;
}


void bicolor(SDL_Surface *img, unsigned blockSize)
{
    unsigned width = img->w;
    unsigned height = img->h;
    
    SDL_Surface* tmp = SDL_CreateRGBSurface(0, width, height, 32, 0, 0 ,0 ,0);
    SDL_BlitSurface(img, NULL, tmp, NULL);

    for(unsigned x = 0; x<width; x++)
    {
	for(unsigned y = 0; y<height; y++)
	{
   	    Uint32 pixel = getPixel(img, x, y);
    	    Uint8 color, null, nul;
    	    SDL_GetRGB(pixel, img->format, &color, &null, &nul);
	    
	    if(mean(tmp, x, y, blockSize)<=(int) color)
	        pixel = SDL_MapRGB(img->format, 255, 255, 255);
	    else
	        pixel = SDL_MapRGB(img->format, 0, 0, 0);

	    setPixel(img, x, y, pixel);
        }
    }
    SDL_FreeSurface(tmp);    
}
