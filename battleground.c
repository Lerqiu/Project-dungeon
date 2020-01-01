#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "battleground.h"
#include "window_creator.h"
#include "map_loader.h"

void create_battleground(char mapName[])
{

    GtkWidget *window = window_creator_create_window();

    gtk_window_set_default_size(GTK_WINDOW(window), DEF_IMAGE_SIZE * 11, DEF_IMAGE_SIZE * 7);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    Prototype_map *pr_map = prototype_load_map(mapName);

    BattlegroundStatic *static_objects_on_map;
    static_objects_on_map = (BattlegroundStatic *)malloc(sizeof(BattlegroundStatic));
    static_objects_on_map->X = pr_map->X;
    static_objects_on_map->Y = pr_map->Y;
    static_objects_on_map->map = (BattlegroundStatic_element **)malloc(sizeof(BattlegroundStatic_element *) * pr_map->Y * pr_map->X);

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            static_objects_on_map->map[i * static_objects_on_map->X + q] = load_battleground_static((pr_map->map + i * pr_map->X + q), q, i, pr_map);
            //printf("Y:%i X:%i type1:%s type2:%s | ",i,q,static_objects_on_map->map[i * static_objects_on_map->X + q]->type,(pr_map->map + i * pr_map->X + q)->type_of_object);
        }
        // putchar('\n');
    }

    GtkWidget *lay = gtk_layout_new(NULL, NULL);
    gtk_layout_set_size(GTK_LAYOUT(lay), DEF_IMAGE_SIZE * 11, DEF_IMAGE_SIZE * 7);
    gtk_container_add(GTK_CONTAINER(window), lay);

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if((static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage !=NULL)
            gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if((static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage !=NULL)
            gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();
}
