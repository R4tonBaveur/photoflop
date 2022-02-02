#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
GtkWidget* Button;
GtkWidget* Window;
GtkWidget* MenuBar;
void on_start(GtkButton *button, gpointer user_data)
{
    g_print("Hello World\n");
}

int main(int argc, char **argv){
    // Initializes GTK.
    gtk_init(NULL, NULL);
    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    Window = GTK_WINDOW(gtk_builder_get_object(builder, "Window"));
    Button = GTK_BUTTON(gtk_builder_get_object(builder, "Button"));
    MenuBar = GTK_WIDGET(gtk_builder_get_object(builder,"MenuBar"));
    gtk_widget_show(Window);
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(Button, "clicked", G_CALLBACK(on_start), NULL);
    // Runs the main loop.
    gtk_main();
    gtk_widget_destroy(Window);
    gtk_widget_destroy(Button);
    gtk_widget_destroy(MenuBar);
    // Exits.
    return 0;
}
