#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "C-Utils/stack/stack.h"
#include "filters/grayscale.h"

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
GtkWidget* GoBackButton;
GtkWidget* OpenButton;
SDL_Surface* Surface;
SDL_Surface* SelectionZone;
struct stack* SurfaceStack;

// Other functions
void UpdateImage(SDL_Surface* newImage){
    printf("Updating image\n");
    Surface = newImage;
    push(SurfaceStack,newImage);
    gtk_widget_queue_draw(DrawingArea);
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
void on_open(){
    printf("Open Clicked\n");
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

void on_GoBack(){
    printf("go back\n");
    //pop(SurfaceStack);
    Surface = pop(SurfaceStack)->data;
    gtk_widget_queue_draw(DrawingArea);
}

void on_GrayScale(){
    printf("GreyScale called\n");
    UpdateImage(GrayScale(Surface,SelectionZone));
}

// Main function
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

    // The zone image
    SelectionZone = SDL_CreateRGBSurface(0,Surface->w,Surface->h,32,0,0,0,0);
    printf("selection zone allocated");
    for(int x=0;x<SelectionZone->w;x++){
        for(int y=0;y<SelectionZone->h;y++){
            setPixel(SelectionZone,x,y,RGBToUint32(SelectionZone,255,255,255));
        }
    }

    // The stack
    SurfaceStack = new_stack();
    push(SurfaceStack,&Surface);

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

    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(OpenButton);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
