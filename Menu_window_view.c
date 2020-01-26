#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#include "Menu_window_functionalities.h"
#include "Menu_window_view.h"
#include "Default_settings.h"
#include "Windows_FIFO.h"

extern char gameName[];
extern char folderPathOthers[];

//Okienko startowe

GtkWidget *windowMain = NULL;
GtkWidget *scrollWindowWidget = NULL;

void createStartWindowMenu(void)
{
    CheckCorrectData_Struct *Data = (CheckCorrectData_Struct *)malloc(sizeof(CheckCorrectData_Struct));

    windowMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    Data->window = windowMain;
    gtk_window_set_title(GTK_WINDOW(windowMain), gameName);
    gtk_window_set_position(GTK_WINDOW(windowMain), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(windowMain), 0);

    scrollWindowWidget = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(windowMain), scrollWindowWidget);

    GtkWidget *boxLMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrollWindowWidget), boxLMain);

    //Nick
    GtkWidget *boxLNick = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLNick);

    GtkWidget *labelNick = gtk_label_new("Nick: ");
    gtk_container_add(GTK_CONTAINER(boxLNick), labelNick);

    GtkWidget *entryNick = gtk_entry_new();
    Data->entryNick = entryNick;
    gtk_container_add(GTK_CONTAINER(boxLNick), entryNick);

    //Server/Host
    GtkWidget *boxLSer_Hos = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLSer_Hos);

    GtkWidget *labelSer_Hos = gtk_label_new("Uruchomić jako server: ");
    gtk_container_add(GTK_CONTAINER(boxLSer_Hos), labelSer_Hos);

    GtkWidget *switchSer_Hos = gtk_switch_new();
    Data->switchSer_Hos = switchSer_Hos;
    gtk_widget_set_hexpand(switchSer_Hos, TRUE);
    gtk_widget_set_halign(switchSer_Hos, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(boxLSer_Hos), switchSer_Hos);
    gtk_switch_set_active(GTK_SWITCH(switchSer_Hos), FALSE);

    //Tryb klawiatury
    GtkWidget *boxLKey = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLKey);

    GtkWidget *labelKey = gtk_label_new("Tryb odwróconej klawiatury: ");
    gtk_container_add(GTK_CONTAINER(boxLKey), labelKey);

    GtkWidget *switchKey = gtk_switch_new();
    Data->switchKey = switchKey;
    gtk_widget_set_hexpand(switchKey, TRUE);
    gtk_widget_set_halign(switchKey, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(boxLKey), switchKey);
    gtk_switch_set_active(GTK_SWITCH(switchKey), FALSE);

    GtkWidget *image1Key = gtk_image_new_from_file(FullName_Path_get(folderPathOthers, "key_r2.png"));
    gtk_container_add(GTK_CONTAINER(boxLMain), image1Key);

    GtkWidget *image2Key = gtk_image_new_from_file(FullName_Path_get(folderPathOthers, "key_s2.png"));
    gtk_container_add(GTK_CONTAINER(boxLMain), image2Key);

    //Mapa
    GtkWidget *boxLMap = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLMap);
    Data->radioButtons = createStartWindowShowMap(boxLMap);
    g_signal_connect(GTK_SWITCH(switchSer_Hos), "notify::active", G_CALLBACK(createStartWindowShowHideMaps), boxLMap);

    //Play
    GtkWidget *boxLPlay = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLPlay);

    GtkWidget *buttonPlay = gtk_button_new_with_label("Play");
    g_signal_connect(G_OBJECT(buttonPlay), "clicked", G_CALLBACK(checkCorrectData), Data);
    gtk_widget_set_hexpand(buttonPlay, TRUE);
    gtk_widget_set_halign(buttonPlay, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(boxLPlay), buttonPlay);

    g_signal_connect(G_OBJECT(windowMain), "destroy", G_CALLBACK(destroyWindow), NULL);
    gtk_window_set_default_size(GTK_WINDOW(windowMain), 400, 240 * 2);

    gtk_widget_show_all(windowMain);
    gtk_widget_hide(boxLMap);
}
