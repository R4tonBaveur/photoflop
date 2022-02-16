#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "random.h"

int main(int argc, char **argv)
{
	SDL_Surface* image = IMG_Load("./images/cat.jpg");
	//Initialize SDL 
	SDL_Init(SDL_INIT_VIDEO);

	// create an SDL window 
	 SDL_Window * window = SDL_CreateWindow("Photoflop",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
	// create a renderer 
	 SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	 bool running = true;
	 SDL_Event event; 
	 while(running)
	 {
		 //Process events 
		 while ( SDL_PollEvent(&event))
		 {
			 if(event.type == SDL_QUIT)
			 {
				 running = false; 
			 }
		 }

		 //clear screen with black 
		 SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		 SDL_RenderClear(renderer);

		 //Draw
		 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Yellow
		 //Draw points
		 //SDL_RenderDrawPoint(renderer,randomNumber(0, image->w), randomNumber(0,image->h));
		 //Draw rectangle 
		 SDL_Rect rect = { 50, 50, 200, 200};
		 SDL_RenderFillRect(renderer, &rect);

		 //Show what was drawn 
		 SDL_RenderPresent(renderer);
	 }

	 //Release ressources 
	 SDL_DestroyRenderer(renderer);
	 SDL_DestroyWindow(window);
	 SDL_Quit();

	 return 0;

}
