#include "image.h"
void dye(SDL_Surface* image,int r,int g,int b){
    int width = image->w;
    int height = image->h;
    Uint32 p;
    Uint8 red,green,blue;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            p = getPixel(image,x,y);
            SDL_GetRGB(p,image->format,&red,&green,&blue);
            red *=(Uint8) r;
            green *=(Uint8) g;
            blue *= (Uint8) b;
            setPixel(image,x,y,RGBToUint32(image,red,green,blue));
        }
    }
}
