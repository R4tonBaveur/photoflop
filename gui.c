#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "C-Utils/stack/stack.h"
#include <stdlib.h>
#include <math.h>

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
GtkWidget* GoBackButton;
GtkWidget* OpenButton;
SDL_Surface* Surface;
SDL_Surface* SelectionZone;
int SurfaceVersion;

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
void on_open(){
    printf("Open Clicked\n");
}

void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    printf("on draw called\n");
    if(Surface)
        printf("Surface initialized\n");
    else{
        printf("No Surface");
        return;
    }
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

void on_GoBack(){
    printf("go back\n");
    char path[64];
    SurfaceVersion--;
    printf("1\n");
    sprintf(path,"%d.jpg\0",SurfaceVersion);
    printf("%s\n",path);
    Surface = IMG_Load(path);
    printf("3%d\n",Surface->w);
    gtk_widget_queue_draw(DrawingArea);
    printf("4\n");
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
                setPixel(Surface, x, y, pixel);
        }
    }
    gtk_widget_queue_draw(DrawingArea);
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
    gtk_widget_queue_draw(DrawingArea);
}

void print_int(void* val){
    int * p = val;
    printf("%d",*p);
}
// Main function
int main(/*int argc, char **argv*/){
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
    Surface = resize(loadImage("images/cat.jpg"));
    if(Surface==NULL){
        printf("Error while loading surface\n");
        return 1;
    }
    SurfaceVersion = 0;
    // The zone image
    SelectionZone = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    printf("selection zone allocated\n");
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
        }
    }
    
    // Getting the different components from the builder
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    OpenButton = GTK_WIDGET(gtk_builder_get_object(builder,"OpenButton"));
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

    // Displaying the window
    gtk_window_set_default_size(GTK_WINDOW(Window),500,500);
    gtk_window_set_resizable(Window, FALSE);    
    gtk_widget_show_all(Window);
    gtk_widget_show(Window);

    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(OpenButton,"activate", G_CALLBACK(on_open), NULL);
    g_signal_connect(DrawingArea,"draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(GoBackButton,"activate",G_CALLBACK(on_GoBack), NULL);
    g_signal_connect(GrayScaleButton,"activate",G_CALLBACK(on_GrayScale), NULL);
    g_signal_connect(SepiaButton,"activate",G_CALLBACK(on_Sepia), NULL);
    g_signal_connect(DarknessButton,"activate",G_CALLBACK(on_Darkness), NULL);
    g_signal_connect(BrightnessButton,"activate",G_CALLBACK(on_Brightness), NULL);

    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(OpenButton);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
