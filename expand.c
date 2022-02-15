#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "math.h"

SDL_Surface* expand_surface(SDL_Surface *img, unsigned to_add)
{
	SDL_Surface *screen;

	unsigned width = to_add*2 + (unsigned) img->w;
	unsigned height = to_add*2 + (unsigned) img->h;
	
    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(width, height 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	
	if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, to_add, to_add, img->w+to_add, img->h+to_add);

    // return the screen for further uses
    return screen;
}

SDL_Surface* expand(SDL_Surface *img, unsigned px_to_add)
{
	SLD_Surface* expanded_img = expand_surface(img, px_to_add);
	unsigned w = (unsigned) img->w;
	unsigned h = (unsigned) img->h;
	
	unsigned width = px_to_add*2 + (unsigned) img->w;
	unsigned height = px_to_add*2 + (unsigned) img->h;
	
	
	for(unsigned x=0; x<width; x++)
	{
		unsigned y
		for(y=0; y<height; y++)
		{
			if(x<px_to_add || x>px_to_add+w)
			{
				if(y<px_to_add || y>px_to_add+w)
				{
					copy_x = ((int) x/w)*w;
					copy_x = ((int) y/h)*h;
					
					Unint32 pixel = get_pixel(*img, copy_x, copy_y);
					put_pixel(expanded_img, x, y, pixel);
				}
				else
				{	
					
				}
			}	
			else if(y<px_to_add || y>px_to_add+w)
			{
				
			}
		}
		if(y+1==px_to_add &&(x>px_to_add && x<px_to_add+w))
			y+=h;
	}

	return expanded_img;
}