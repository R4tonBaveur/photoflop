#ifndef ROTATION_OPERATIONS_H
#define ROTATION_OPERATIONS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "search.h"
#include "queue/queue.h"

////Manual Rotation////

void large_copy_image(SDL_Surface *img, SDL_Surface *image_copy);
void rotate_image(SDL_Surface *img, SDL_Surface *new_img, int angle);

/////Automatic Rotation////

int Get_angle(struct extremities *e);
void Update_coord_rotated(struct coord *c, double angle,
		long width, long height, long size);
void Update_extr_rotated(struct extremities *e, int angle,
		long width, long height, long side);
int Get_width_extr(struct extremities *e);
int Get_height_extr(struct extremities *e);
SDL_Surface* Rescale_image(SDL_Surface* img,
		struct extremities *e);

#endif
