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
SDL_Surface* Surface;
int Zoom = 50;
// Callback Functions
void on_open(){
    printf("Open Clicked\n");
}

void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double step = (double) 100/Zoom;
    int fillSize = 1;
    if(Zoom>100){
        fillSize = Zoom/100;
    }
    printf("on draw called\n");
    int width, height;
    Uint8 r,g,b;
    Uint32 C;
    width = Surface->w;//gtk_widget_get_allocated_width(widget);
    height = Surface->h;//gtk_widget_get_allocated_height(widget);
    for(double x=0;x<width;x+=step){
        for(double y=0;y<height;y+=step){
            C = getPixel(Surface, (int) x, (int) y);
            SDL_GetRGB(C, Surface->format,&r, &g, &b);
            cairo_set_source_rgb(cr,
                    (double) r/(double) 255,
                    (double) g/(double) 255,
                    (double) b/(double) 255);
            cairo_rectangle(cr, (int) x/step,(int) y/step, fillSize , fillSize);
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
    Surface = loadImage("images/cat.jpg");
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
    
    // Displaying the window
    gtk_window_set_default_size(GTK_WINDOW(Window),Surface->w+20,Surface->h+20);
    gtk_widget_show_all(Window);
    gtk_widget_show(Window);

    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(Open,"activate", G_CALLBACK(on_open), NULL);
    g_signal_connect(DrawingArea,"draw", G_CALLBACK(on_draw), NULL);


    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(Open);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
