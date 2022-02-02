#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

GtkWidget* gtk_image_new_from_sdl_surface(SDL_Surface *surface)
{
    Uint32 src_format;
    Uint32 dst_format;
    GdkPixbuf *pixbuf;
    gboolean has_alpha;
    int rowstride;
    guchar *pixels;
    GtkWidget *image;
    // select format                                                            
    src_format = surface->format->format;
    has_alpha = SDL_ISPIXELFORMAT_ALPHA(src_format);
    if (has_alpha) {
        dst_format = SDL_PIXELFORMAT_RGBA32;
    } else {
        dst_format = SDL_PIXELFORMAT_RGB24;
    }
    // create pixbuf                                                            
    pixbuf = gdk_pixbuf_new (
            GDK_COLORSPACE_RGB, has_alpha, 8,
            surface->w, surface->h);
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);
    // copy pixels                                                              
    SDL_LockSurface(surface);
    SDL_ConvertPixels (
            surface->w, surface->h, src_format,
            surface->pixels, surface->pitch,
            dst_format, pixels, rowstride);
    SDL_UnlockSurface(surface);
    // create GtkImage from pixbuf                                              
    image = gtk_image_new_from_pixbuf (pixbuf);
    // release our reference to the pixbuf                                      
    g_object_unref (pixbuf);
    return image;
}


// GTK items
GtkWidget* Window;
GtkWidget* MenuBar;
GtkWidget* Open;
GtkWidget* Image;
SDL_Surface* Surface;

// Callback Functions
void on_open(){
    printf("Open Clicked\n");
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

    // Getting the different components from the builder
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    Open = GTK_WIDGET(gtk_builder_get_object(builder,"Open"));
    Image = GTK_WIDGET(gtk_builder_get_object(builder,"Image"));
    Image = gtk_image_new_from_sdl_surface(Surface);
    // Displaying the window
    gtk_widget_show(Window);
    
    // Connecting the signals
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(Open,"activate", G_CALLBACK(on_open), NULL);

    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(Open);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
