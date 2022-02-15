#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

// Expands the image of a number of pixels on each side of the image
/*
	img The image to expand
	px_to_add The number of pixels to add
	
	the function don't work when px_to_add>=length or width of img
*/
SDL_Surface* expand(SDL_Surface *img, unsigned px_to_add);
