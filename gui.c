#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "C-Utils/stack/stack.h"
#include <stdlib.h>
#include <math.h>
struct SStack{
    struct SStack* old;
    SDL_Surface* surface;
};
// GTK items
GtkWidget* Window;
GtkWidget* MenuBar;
GtkWidget* Selection;
GtkWidget* Filters;
GtkWidget* Drawing;
GtkWidget* Actions;
GtkWidget* DrawingArea;
GtkWidget* Layout; 
GtkWidget* GrayScaleButton;
GtkWidget* SepiaButton;
GtkWidget* DarknessButton;
GtkWidget* BrightnessButton;
GtkWidget* BlurButton;
GtkWidget* GoBackButton;
GtkWidget* SaveButton;
GtkWidget* RectangleButton;
struct SStack* s;
SDL_Surface* Surface;
SDL_Surface* SelectionZone;
char* filename;

void get_coord(SDL_Surface* image, size_t *x, size_t *y)
{
    static int counter = 0;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Photoflop",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
        image);

    int quit = 0;
    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_MOUSEBUTTONDOWN:
            *x = event.motion.x;
            *y = event.motion.y;
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

// Resize the Surface to fit the app
SDL_Surface* resize(SDL_Surface* image){
    int target = 500;
    int width = target;
    int height = target;
    if(image->w>image->h){
        height = image->h*target/image->w;
    } else {
        width = image->w*target/image->h;
    }
    SDL_Surface* res = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            setPixel(res,x,y,
                    getPixel(image,x*image->w/target,y*image->h/target));
        }
    }
    SDL_FreeSurface(image);
    return res;
}

// Callback Functions

void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    if(!Surface)
        return;
    int width, height;
    Uint8 r,g,b;
    Uint32 C;
    width = Surface->w;
    height = Surface->h;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            C = getPixel(Surface, x, y);
            SDL_GetRGB(C, Surface->format,&r, &g, &b);
            cairo_set_source_rgb(cr,
                    (double) r/(double) 255,
                    (double) g/(double) 255,
                    (double) b/(double) 255);
            cairo_rectangle(cr, x, y, 1 , 1);
            cairo_fill(cr);
        }
    }
    cairo_stroke(cr);
}
void update(){
    struct SStack* new = malloc(sizeof(struct SStack));
    SDL_Surface* newSurface = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    for(int x=0;x<Surface->w;x++){
        for(int y=0;y<Surface->h;y++){
            setPixel(newSurface,x,y,getPixel(Surface,x,y));
        }
    }
    new->old = s;
    new->surface = newSurface;
    s = new;
}

void on_GoBack(){
    s = s->old;
    Surface = s->surface;
    gtk_widget_queue_draw(DrawingArea);
}

void on_GrayScale(){
    int width = Surface->w;
    int height = Surface->h;
    for(int x = 0; x<width; x++)
    {
        for(int y = 0; y<height; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            Uint8 avg = 0.3*r+0.59*g+0.11*b;
            pixel = SDL_MapRGB(Surface->format, avg, avg, avg);
            if(getPixel(SelectionZone,x,y))
                setPixel(Surface, x, y, pixel);
        }
    }
    gtk_widget_queue_draw(DrawingArea);
    update();
}

void on_Sepia(){
    for(int x = 100; x < Surface->w-200; x++)
    {
        for(int y = 300; y < Surface->h-30; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(Surface->format, r * 0.393 + g * 0.769 + b * 0.189, r * 0.349 + g * 0.686 + b * 0.168, r * 0.272 + g * 0.534 + b * 0.131);
            setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);

}

void on_Darkness(){
    int n = 5;
    for(size_t x = 0; x < Surface->w; x++)
    {
        for(size_t y = 0; y < Surface->h; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(Surface->format, (int) (255 * pow((double) r/255, n)), (int) (255 * pow((double) g/255, n)), (int) (255 * pow((double) b/255, n)));
            setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Brightness(){
    float n = 0.5f;
    for(size_t x = 0; x < Surface->w; x++)
    {
        for(size_t y = 0; y < Surface->h; y++)
        {
            Uint32 pixel = getPixel(Surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, Surface->format, &r, &g, &b);
            pixel = SDL_MapRGB(Surface->format, (int) (255 * pow((double) r/255, n)), (int) (255 * pow((double) g/255, n)), (int) (255 * pow((double) b/255, n)));
            setPixel(Surface, x, y, pixel);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_Blur(){
    SDL_Surface* image = Surface;
    int precision = 2;
    Uint32 matrix[image->w][image->h];
    for(int x=0;x<image->w;x++){
        for(int y=0;y<image->h;y++){
            Uint32 totalPixels = 0;
            Uint32 colorSum = 0;
            for(int xShift=-precision;xShift<precision;xShift++){
                for(int yShift=-precision;yShift<precision;yShift++){
                    if(x+xShift>=0&&x+xShift<image->w&&y+yShift>=0&&y+yShift<image->h){
                        totalPixels++;
                        uint8_t val;
                        SDL_GetRGB(getPixel(image,xShift+x,yShift+y), image->format, &val, &val, &val);
                        colorSum+=val;
                    }
                }
            }
            Uint32 grey = colorSum/totalPixels;
            uint8_t val;
            SDL_GetRGB(getPixel(image,x,y), image->format, &val, &val, &val);
            matrix[x][y] = RGBToUint32(image,grey,grey,grey);
        }
    }
    for(int x=0;x<image->w;x++){
        for(int y=0;y<image->h;y++){
            setPixel(image,x,y,matrix[x][y]);
        }
    }
    update();
    gtk_widget_queue_draw(DrawingArea);
}

void on_save(){
    char path[64];
    sprintf(path,"exports/%s",filename);
    saveImage(Surface,path);
}

void on_Rectangle(){
    size_t x1,y1,x2,y2;
    get_coord(Surface,&x1,&y1);
    get_coord(Surface,&x2,&y2);
    printf("%zu\n",x1);
    printf("%zu\n",y1);
    printf("%zu\n",x2);
    printf("%zu\n",y2);
    if(x2<x1){
        size_t mem = x1;
        x1 = x2;
        x2 = mem;
    }
    if(y2<y1){
        size_t mem = y1;
        y1 = y2;
        y2 = mem;
    }
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            if(x>=x1 && x<=x2 && y>=y1 && y<=y2){
                setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
            } else {
                setPixel(SelectionZone,x,y,0);
            }
        }
    }
}
// Main function
int main(int argc, char **argv){
    if(argc<2){
        errx(1,"Usage: ./gui <filename>");
    }
    // Initializes GTK
    gtk_init(NULL, NULL);

    // Loads builder from XML file
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // The image
    filename = argv[1];
    Surface = resize(loadImage(filename));
    if(Surface==NULL){
        printf("Error while loading surface\n");
        return 1;
    }
    // The zone image
    SelectionZone = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    printf("selection zone allocated\n");
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
        }
    }
    
    // Surface stack
    s = malloc(sizeof(struct SStack));
    s->old = NULL;
    s->surface = Surface;
    update();

    // Getting the different components from the builder
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    SaveButton = GTK_WIDGET(gtk_builder_get_object(builder,"SaveButton"));
    Layout = GTK_WIDGET(gtk_builder_get_object(builder,"BoxLayout"));
    Selection = GTK_WIDGET(gtk_builder_get_object(builder,"Selection"));
    Filters = GTK_WIDGET(gtk_builder_get_object(builder,"Filters"));
    Drawing = GTK_WIDGET(gtk_builder_get_object(builder,"Drawing"));
    Actions = GTK_WIDGET(gtk_builder_get_object(builder,"Actions"));
    DrawingArea = GTK_WIDGET(gtk_builder_get_object(builder,"DrawingArea"));    
    GrayScaleButton = GTK_WIDGET(gtk_builder_get_object(builder,"GrayScaleButton"));
    SepiaButton = GTK_WIDGET(gtk_builder_get_object(builder,"SepiaButton"));
    DarknessButton = GTK_WIDGET(gtk_builder_get_object(builder,"DarknessButton"));
    BrightnessButton = GTK_WIDGET(gtk_builder_get_object(builder,"BrightnessButton"));
    GoBackButton = GTK_WIDGET(gtk_builder_get_object(builder,"GoBackButton"));
    BlurButton = GTK_WIDGET(gtk_builder_get_object(builder,"BlurButton"));
    RectangleButton = GTK_WIDGET(gtk_builder_get_object(builder,"RectangleButton"));
    // Displaying the window
    gtk_window_set_default_size(GTK_WINDOW(Window),500,500);
    gtk_window_set_resizable(Window, FALSE);    
    gtk_widget_show_all(Window);
    gtk_widget_show(Window);

    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(SaveButton,"activate", G_CALLBACK(on_save), NULL);
    g_signal_connect(DrawingArea,"draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(GoBackButton,"activate",G_CALLBACK(on_GoBack), NULL);
    g_signal_connect(GrayScaleButton,"activate",G_CALLBACK(on_GrayScale), NULL);
    g_signal_connect(SepiaButton,"activate",G_CALLBACK(on_Sepia), NULL);
    g_signal_connect(DarknessButton,"activate",G_CALLBACK(on_Darkness), NULL);
    g_signal_connect(BrightnessButton,"activate",G_CALLBACK(on_Brightness), NULL);
    g_signal_connect(BlurButton,"activate",G_CALLBACK(on_Blur), NULL);
    g_signal_connect(RectangleButton,"activate",G_CALLBACK(on_Rectangle), NULL);

    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
