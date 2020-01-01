#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "window_creator.h"


GtkWidget *window_creator_create_window(void)
{
    gtk_init(NULL, NULL);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Save the pronces!!!");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

void destroy_window(GtkWidget *window)
{
    gtk_main_quit();
    if (window != NULL)
        g_object_unref(window);
}
