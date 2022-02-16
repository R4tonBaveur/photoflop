#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

// GTK items
GtkWidget* Window;
GtkWidget* MenuBar;
GtkWidget* Open;
GtkWidget* Selection;
GtkWidget* Filters;
GtkWidget* Drawing;
GtkWidget* Actions;
GtkWidget* DrawingArea;
GtkWidget* Layout; 
GtkWidget* GrayScale;
SDL_Surface* Surface;


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
void on_GrayScale(){
    /*
     * Needs to be replaced by importing filter
     */
    int width = Surface->w;
    int height = Surface->h;
    Uint32 p;
    Uint8 r,g,b,avg;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            p = getPixel(Surface,x,y);
            SDL_GetRGB(p,Surface->format,&r,&g,&b);
            avg = 0.3*r + 0.59*g + 0.11*b;
            p = SDL_MapRGB(Surface->format,avg,avg,avg);
            setPixel(Surface, x, y,p);
        }
    }
    gtk_widget_queue_draw(DrawingArea);
}
void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    printf("on draw called\n");
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


int main(int argc, char **argv){
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

    // Getting the different components from the builder
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    Open = GTK_WIDGET(gtk_builder_get_object(builder,"Open"));
    Layout = GTK_WIDGET(gtk_builder_get_object(builder,"BoxLayout"));
    Selection = GTK_WIDGET(gtk_builder_get_object(builder,"Selection"));
    Filters = GTK_WIDGET(gtk_builder_get_object(builder,"Filters"));
    Drawing = GTK_WIDGET(gtk_builder_get_object(builder,"Drawing"));
    Actions = GTK_WIDGET(gtk_builder_get_object(builder,"Actions"));
    DrawingArea = GTK_WIDGET(gtk_builder_get_object(builder,"DrawingArea"));    
    GrayScale = GTK_WIDGET(gtk_builder_get_object(builder,"GrayScale"));

    // Displaying the window
    gtk_window_set_default_size(GTK_WINDOW(Window),500,500);
    gtk_window_set_resizable(Window, FALSE);    
    gtk_widget_show_all(Window);
    gtk_widget_show(Window);

    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(Open,"activate", G_CALLBACK(on_open), NULL);
    g_signal_connect(DrawingArea,"draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(GrayScale,"activate", G_CALLBACK(on_GrayScale), NULL);


    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(Open);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
