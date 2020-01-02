#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "battleground.h"
#include "window_creator.h"
#include "map_loader.h"
#include "objects_events.h"

BattlegroundStatic *static_objects_on_map;
BattlegroundDynamic *dynamic_objects_on_map;

extern int DEF_IMAGE_SIZE;
extern bool reverseKeyBoard;

extern GtkAdjustment *hadj;
extern GtkAdjustment *vadj;

extern int windowWidth;
extern int windowHeight;

extern int mapRows;
extern int mapColumns;

BattlegroundDynamic_element *mainCharacter;

static void create_battleground_static(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay)
{
    static_objects_on_map = (BattlegroundStatic *)malloc(sizeof(BattlegroundStatic));
    static_objects_on_map->X = pr_map->X;
    static_objects_on_map->Y = pr_map->Y;
    static_objects_on_map->map = (BattlegroundStatic_element **)malloc(sizeof(BattlegroundStatic_element *) * pr_map->Y * pr_map->X);

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            static_objects_on_map->map[i * static_objects_on_map->X + q] = load_battleground_static((pr_map->map + i * pr_map->X + q), q, i, pr_map);
        }
    }

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if ((static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage != NULL)
                gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }
}

static void create_battleground__static_top(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay)
{

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if ((static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage != NULL)
                gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }
}

static void create_character(BattlegroundDynamic *map)
{

    BattlegroundDynamic_element *character[2];

    int b = 0;
    for (int a = 0; a < map->amount; a++)
    {
        if (!strcmp(map->tabOfElements[a]->type, "character"))
        {
            character[b] = map->tabOfElements[a];
            b++;
        }
    }
    extern int characterServerIndexX;
    extern int characterServerIndexY;
    extern char *characterImagePathServer;
    extern char *characterImagePathHost;

    for (int i = 0; i < 2; i++)
    {
        if (character[i]->indexStartPointX == characterServerIndexX && character[i]->indexStartPointY == characterServerIndexY)
        {
            character[i]->image = gtk_image_new_from_file(characterImagePathServer);
            mainCharacter = character[i];
            mainCharacter->objectData = (void *)malloc(sizeof(CharacterData));
            CharacterData *data = (CharacterData *)(mainCharacter->objectData);
            data->hadj = NULL;
            data->vadj = NULL;
        }
        else
        {
            character[i]->image = gtk_image_new_from_file(characterImagePathHost);
        }
        //printf("X%i %i Y:%i %i\n", character[i]->indexStartPointX, characterServerIndexX, character[i]->indexStartPointY, characterServerIndexY);
    }
}

static void create_battleground_dynamic(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay)
{
    dynamic_objects_on_map = (BattlegroundDynamic *)malloc(sizeof(BattlegroundDynamic));
    dynamic_objects_on_map->amount = amount_of_dynamic_elements(pr_map);
    dynamic_objects_on_map->tabOfElements = (BattlegroundDynamic_element **)malloc(sizeof(BattlegroundDynamic_element *) * dynamic_objects_on_map->amount);

    int a = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            dynamic_objects_on_map->tabOfElements[a] = NULL;
            dynamic_objects_on_map->tabOfElements[a] = load_battleground_dynamic((pr_map->map + i * pr_map->X + q), q, i);
            if (dynamic_objects_on_map->tabOfElements[a] != NULL)
                a++;
        }
    }

    create_character(dynamic_objects_on_map);
    for (int a = 0; a < dynamic_objects_on_map->amount; a++)
    {
        if (dynamic_objects_on_map->tabOfElements[a]->image != NULL)
        {
            gtk_layout_put(GTK_LAYOUT(lay), dynamic_objects_on_map->tabOfElements[a]->image, dynamic_objects_on_map->tabOfElements[a]->posX, dynamic_objects_on_map->tabOfElements[a]->posY);
            dynamic_objects_on_map->tabOfElements[a]->layout = lay;
        }
    }
}

void create_battleground(char mapPath[])
{
    GtkWidget *window = window_creator_create_window();

    gtk_window_set_default_size(GTK_WINDOW(window), windowWidth, windowHeight);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    hadj = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    vadj = gtk_adjustment_new(0, 0, 0, 0, 0, 0);

    GtkWidget *lay = gtk_layout_new(hadj, vadj);

    gtk_layout_set_size(GTK_LAYOUT(lay),mapRows*DEF_IMAGE_SIZE, mapColumns*DEF_IMAGE_SIZE);
    gtk_container_add(GTK_CONTAINER(window), lay);

    Prototype_map *pr_map = prototype_load_map(mapPath);

    create_battleground_static(window, pr_map, lay);

    create_battleground_dynamic(window, pr_map, lay);

    create_battleground__static_top(window, pr_map, lay);

    set_view_center_By_Character((void*)mainCharacter);

    //sterowanie postacią
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

    if (reverseKeyBoard == true)
    {
        g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(objects_movie_Key_Reverse), mainCharacter);
    }
    else
    {
        g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(objects_movie), mainCharacter);
    }

    gtk_widget_show_all(window);
    gtk_main();
}
