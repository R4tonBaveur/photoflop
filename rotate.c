#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image_operations.h"
#include "pixel_operations.h"
#include "filters.h"

void Rotate_manual(char* path, int angle)
{
	// initialize image
 	// Declaration and Initialisation
	SDL_Surface* image_surface;
	SDL_Surface* new_image_surface;
	SDL_Surface* image_copy;
	SDL_Surface* screen_surface;
	init_sdl();

	//Load image
	image_surface = load_image(path);
	screen_surface = display_image(image_surface);

	wait_for_keypressed();

	//Some Good Variables
	long h = image_surface->h;
	long w = image_surface->w;
	long diagonal = sqrt(h*h + w*w);

	//Copy image_surface into a bigger image
	image_copy = SDL_CreateRGBSurface(0, diagonal, diagonal, 32, 0, 0, 0, 0);
	large_copy_image(image_surface, image_copy);
	printf("Copying image : Done\n");
	SDL_FreeSurface(image_surface);

	//Execute the rotation
	new_image_surface = SDL_CreateRGBSurface(0,
			diagonal, diagonal, 32, 0, 0, 0, 0);
	screen_surface = display_image(new_image_surface);
	rotate_image(image_copy, new_image_surface, angle);
	update_surface(screen_surface, new_image_surface);
	printf("Rotation : Done\n");

	wait_for_keypressed();

	//Save Image
	Save_Image(new_image_surface, "Images/rotate_manual.bmp");
	Save_Image(new_image_surface, path);

	//Free Useless Memories
	SDL_FreeSurface(new_image_surface);
	SDL_FreeSurface(image_copy);
}

/*int main()
{
	// initialize image
 	// Declaration and Initialisation
	SDL_Surface* image_surface;
	SDL_Surface* new_image_surface;
	SDL_Surface* screen_surface;
	SDL_Surface* image_copy;

	init_sdl();

	//Load image
	image_surface = load_image("Images/my_image3.jpg");
	screen_surface = display_image(image_surface);

	wait_for_keypressed();

	//Some Good Variables
	long h = image_surface->h;
	long w = image_surface->w;
	long diagonal = sqrt(h*h + w*w);

	//Copy image_surface into a bigger image
	image_copy = SDL_CreateRGBSurface(0, diagonal,
	diagonal, 32, 0, 0, 0, 0);
	SDL_FillRect(image_copy, NULL,
	SDL_MapRGB(image_copy->format, 255, 255, 255));
	image_copy = large_copy_image(image_surface, image_copy);
	printf("Copying image : Done\n");
	SDL_FreeSurface(image_surface);

	//Create new surface
	new_image_surface = SDL_CreateRGBSurface(0,
	diagonal, diagonal, 32, 0, 0, 0, 0);
	SDL_FillRect(new_image_surface, NULL,
	SDL_MapRGB(image_copy->format, 255, 255, 255));
	screen_surface = display_image(new_image_surface);

	//for (size_t m = 0; m < 720; m++)
	//{
	//Execute the rotation
	new_image_surface = rotate_image(image_copy, new_image_surface, 45);
	update_surface(screen_surface, new_image_surface);
	printf("Rotation : Done\n");
	//image_copy = new_image_surface;
	//}

	wait_for_keypressed();

	//Save Image
	Save_Image(new_image_surface, "Images/new_image.bmp");

	//Free Useless Memories
	SDL_FreeSurface(new_image_surface);
	SDL_FreeSurface(image_copy);
	SDL_FreeSurface(screen_surface);

	return 0;
}*/
