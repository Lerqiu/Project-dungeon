#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>

#include "window_creator.h"
#include "settings.h"

extern char gameName[];
extern int maxLengthOfPath;

GtkWidget *window_creator_create_window(void)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //gtk_window_set_title(GTK_WINDOW(window), gameName);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}



