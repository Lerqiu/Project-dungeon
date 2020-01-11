#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>

#include "window_creator.h"
#include "settings.h"

extern char gameName[];
extern int maxLengthOfPath;
extern char folderPathMaps[];
extern char folderPathOthers[];

GtkWidget *window_creator_create_window(void)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //gtk_window_set_title(GTK_WINDOW(window), gameName);
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

typedef struct map_list
{
    int amount;
    GtkWidget **list;
} mapList;

mapList *createStartWindowShowMap(GtkWidget *p)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(folderPathMaps);
    int amount = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name))
            {
                amount++;
                //printf("%s\n", dir->d_name);
                //GtkWidget *button = gtk_check_button_new_with_label(dir->d_name);
                //gtk_container_add(GTK_CONTAINER(p), button);
                //gtk_widget_show(button);
            }
        }
        closedir(d);
    }

    mapList *list = (mapList *)malloc(sizeof(mapList));
    list->amount = amount;
    list->list = (GtkWidget **)malloc(sizeof(GtkWidget *) * amount);

    d = opendir(folderPathMaps);
    amount = 0;
    GSList *glist = NULL;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp("..", dir->d_name) && strcmp(".", dir->d_name))
            {
                //list->list[amount] = gtk_check_button_new_with_label(dir->d_name);
                list->list[amount] = gtk_radio_button_new_with_label(glist, dir->d_name);
                glist =gtk_radio_button_get_group (GTK_RADIO_BUTTON(list->list[amount]));
                gtk_container_add(GTK_CONTAINER(p), list->list[amount]);
                gtk_widget_show(list->list[amount]);
                gtk_widget_set_hexpand(list->list[amount], TRUE);
                gtk_widget_set_halign(list->list[amount], GTK_ALIGN_CENTER);
                //printf("%s\n", dir->d_name);
                //GtkWidget *button = gtk_check_button_new_with_label(dir->d_name);
                //gtk_container_add(GTK_CONTAINER(p), button);
                //gtk_widget_show(button);
                amount++;
            }
        }
        closedir(d);
    }
    gtk_widget_hide(p);
    return list;
}

gboolean createStartWindowShowHideMaps(GtkWidget *widget, GParamSpec *pspec, gpointer data)
{
    GtkWidget *p = (GtkWidget *)(data);
    if (gtk_switch_get_active(GTK_SWITCH(widget)))
    {
        //printf("Tryb on\n");
        //createStartWindowShow(p);
        gtk_widget_show(p);
    }
    else
    {
        //printf("Tryb off\n");
        gtk_widget_hide(p);
    }

    return TRUE;
}

void createStartWindow(void)
{

    gtk_init(NULL, NULL);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), gameName);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(window), scroll);

    GtkWidget *boxLMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scroll), boxLMain);

    //Nick
    GtkWidget *boxLNick = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLNick);

    GtkWidget *labelNick = gtk_label_new("Nick: ");
    gtk_container_add(GTK_CONTAINER(boxLNick), labelNick);

    GtkWidget *entryNick = gtk_entry_new();
    //gtk_widget_set_hexpand(entryNick, TRUE);
    //gtk_widget_set_halign(entryNick, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(boxLNick), entryNick);

    //Server/Host
    GtkWidget *boxLSer_Hos = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLSer_Hos);

    GtkWidget *labelSer_Hos = gtk_label_new("Uruchomić jako server: ");
    gtk_container_add(GTK_CONTAINER(boxLSer_Hos), labelSer_Hos);

    GtkWidget *switchSer_Hos = gtk_switch_new();
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
    gtk_widget_set_hexpand(switchKey, TRUE);
    gtk_widget_set_halign(switchKey, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(boxLKey), switchKey);
    gtk_switch_set_active(GTK_SWITCH(switchKey), FALSE);

    GtkWidget *image1Key = gtk_image_new_from_file(FullName_Path_get(folderPathOthers,"key_r2.png"));
    gtk_container_add(GTK_CONTAINER(boxLMain), image1Key);

    GtkWidget *image2Key = gtk_image_new_from_file(FullName_Path_get(folderPathOthers,"key_s2.png"));
    gtk_container_add(GTK_CONTAINER(boxLMain), image2Key);

    //Mapa
    GtkWidget *boxLMap = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLMap);
    mapList *list = createStartWindowShowMap(boxLMap);
    g_signal_connect(GTK_SWITCH(switchSer_Hos), "notify::active", G_CALLBACK(createStartWindowShowHideMaps), boxLMap);

    //Play
    GtkWidget *boxLPlay = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(boxLMain), boxLPlay);

    GtkWidget *buttonPlay = gtk_button_new_with_label("Play");
    gtk_widget_set_hexpand(buttonPlay, TRUE);
    gtk_widget_set_halign(buttonPlay, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(boxLPlay), buttonPlay);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 240*2);

    gtk_widget_show_all(window);
    gtk_widget_hide(boxLMap);
    gtk_main();
}
