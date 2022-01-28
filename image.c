#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/*
 * Returns the value of the color of the pixel in the image at x,y
 * image: the image
 * x,y: coordinates of the pixel
 */
Uint32 getPixel(SDL_Surface *image,uint16_t x,uint16_t y){
    int bpp = image->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;
    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}
/*
 * Sets the value of the color of the pixel in the image at x,y
 * image: the image
 * x,y: coordinates of the pixel
 */
void setPixel(SDL_Surface *image,uint16_t x,uint16_t y,Uint32 color){
    if(x>image->w||y>image->h){
        //printf("setPixel: You are trying to access a pixel outside of the image\n");
        return;
    }
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) image->pixels
                                              + y * image->pitch
                                              + x * image->format->BytesPerPixel);
    *target_pixel = color;
}
/*
 * Converts a RGB value into a Uint32, understandable by SDL
 * image: the image for witch we want to convert the value (needed for the color format)
 * R,G,B: the values of the color
 */
Uint32 RGBToUint32(SDL_Surface *image,uint8_t R,uint8_t G,uint8_t B){
    return SDL_MapRGB(image->format,R,G,B);
}
/*
 * Returns a SDL_Surface* corresponding to the image in the given path
 * file: image path
 */
SDL_Surface* loadImage(char* file){
    return IMG_Load(file);
}
/*
 * Saves a SDL_Surface* corresponding to the given path
 * image:image to be saved
 * file: path to save the image
 */
void saveImage(SDL_Surface* image,char* file){
    SDL_SaveBMP(image,file);
}
/*
 * Displays the image on the screen until we close the opened window
 * image:image to be displayed
 */
void displaySurface(SDL_Surface* image){
    int quit = 0;
    SDL_Event event;
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             image->w,image->h,0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
                                                         image);
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
