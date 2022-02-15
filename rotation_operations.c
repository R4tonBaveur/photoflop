#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

// SDL
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

// our file
#include "image_operations.h"
#include "search.h"
#include "rotation_operations.h"
#include "queue/queue.h"
#include "filters.h"

////////////////Manual Rotation/////////////////////

//Create a copy of img with img's diagonal size
//img is copied in the middle of image_copy
void large_copy_image(SDL_Surface *img, SDL_Surface *image_copy)
{
	//Some good variables
	long size = image_copy->h;
	Uint32 pixel;
	long h = img->h;
	long w = img->w;

	//Update pixels
	for (size_t i = 0; i < (size_t)w; i++)
		for (size_t k = 0; k < (size_t)h; k++)
		{
			pixel = get_pixel(img, i, k);
			put_pixel(image_copy, i+(size-w)/2, k+(size-h)/2, pixel);
		}
}

//Rotate Image according to a given angle
void rotate_image(SDL_Surface *img, SDL_Surface *new_img, int angle)
{
	//Some Good Variables
	Uint32 pixel;
	size_t height = new_img->h;
	size_t width = new_img->w;
	long h = height;
	long w = width;
	double t = angle*M_PI/180; //Angle in radiant

	//Update pixels
	for (size_t i = 0; i < (size_t)w; i++) //x axe
	{
		for (size_t k = 0; k < (size_t)h; k++) //y axe
		{
			long a = i-w/2;//To prevent convertion problems
			long b = k-h/2;//
			long x = cos(t)*a + sin(t)*b + w/2;
			long y = -sin(t)*a + cos(t)*b + h/2;
			if (x >= 0 && y >= 0 && x < w && y < h)
			{
				pixel = get_pixel(img, x, y);
				put_pixel(new_img, i, k, pixel);
			}
		}
	}
}

//Return the angle in degree to have
//the sudoku in a horizontal and vertical angle
int Get_angle(struct extremities *e)
{
	//Many good variables
	struct coord *min_x = &e->min_x;
	struct coord *min_y = &e->min_y;

	int x = min_x->x;
	int y = min_x->y;
	int a = min_y->x;
	int b = min_y->y;

	//Operations to find angle
	int cote_adj = a-x;
	int cote_opp = y-b;
	//printf("adj side = %i and opp side = %i\n",cote_adj,cote_opp);

	double hyp = sqrt(cote_adj*cote_adj + cote_opp*cote_opp);
	//printf("hyp = %f\n",hyp);

	int angle1 = acos((double)cote_adj/hyp)*180/M_PI;
	int angle2 = acos((double)cote_opp/hyp)*180/M_PI;
	//printf("angle cos = %i and angle sin = %i\n",angle1,angle2);

	if (angle1 < angle2)
		return angle1+1;//The +1 is for the loss of data with an int
	return -angle2;
}

//Update the coordinates according to angle of rotation of the image
void Update_coord_rotated(struct coord *c, double angle,
		long width, long height, long size)
{
	long a = c->x - width/2;//To prevent convertion problems
	long b = c->y - height/2;//
	int new_x = cos(angle)*a - sin(angle)*b + width/2 + (size-width)/2;
	int new_y = sin(angle)*a + cos(angle)*b + height/2 + (size-height)/2;
	c->x = new_x;
	c->y = new_y;
}

//Update the extremities according to angle of rotation of the image
void Update_extr_rotated(struct extremities *e, int angle,
		long width, long height, long side)
{
	double t = angle*M_PI/180; //Angle in radiant
	Update_coord_rotated(&e->min_x, t, width, height, side);
	Update_coord_rotated(&e->min_y, t, width, height, side);
	Update_coord_rotated(&e->max_x, t, width, height, side);
	Update_coord_rotated(&e->max_y, t, width, height, side);
}

//Update x and y axes of a coord structure
void update_coords(struct coord *c, int x, int y)
{
	c->x = x;
	c->y = y;
}

//Get width with extremities
int Get_width_extr(struct extremities *e)
{
	struct coord *min_x = &e->min_x;
    struct coord *max_x = &e->max_x;

	int width = max_x->x - min_x->x;
	return width;
}

//Get height with extremities
int Get_height_extr(struct extremities *e)
{
	struct coord *min_y = &e->min_y;
	struct coord *max_y = &e->min_y;

	int height = max_y->y-min_y->y;
	return height;
}

SDL_Surface* Rescale_image(
		SDL_Surface* img, struct extremities *e)
{
	//Many Good Variables
	Uint32 pixel;
	size_t h_t = img->h;
	size_t w_t = img->w;

	struct coord *min_x = &e->min_x;
	struct coord *min_y = &e->min_y;
	struct coord *max_x = &e->max_x;
	struct coord *max_y = &e->max_y;

	int x = min_x->x-3;       //To make code clear to read
	int y = min_y->y-3;       //
	int mx = max_x->x+3;      //
	int my = max_y->y+3;      //

	//New Scale
	int width = mx - x;
	int height = my - y;

	//Create a new black surface
	SDL_Surface *img_scl =
		SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(img_scl, NULL,
		SDL_MapRGB(img_scl->format, 255, 255, 255));

	//Update all pixels
	for (size_t i = 0; i < w_t; i++) //x axe
		for (size_t k = 0; k < h_t; k++) //y axe
			if ((int)i >= x && (int)i < mx &&
				(int)k >= y && (int)k < my)
				{
						pixel = get_pixel(img, i, k);
						put_pixel(img_scl, i-x, k-y, pixel);
				}

	//Update Extremities
	update_coords(&e->min_x, 0, min_x->y - y);
	update_coords(&e->min_y, min_y->x - x, 0);
	update_coords(&e->max_x, width, max_x->y - y);
	update_coords(&e->max_y, max_y->x - x, height);

	return img_scl;
}
