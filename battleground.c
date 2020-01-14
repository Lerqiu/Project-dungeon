#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "battleground.h"
#include "window_creator.h"
#include "map_loader.h"
#include "objects_events.h"
#include "settings.h"
#include "connection.h"

BattlegroundStatic *static_objects_on_map;
BattlegroundDynamic *dynamic_objects_on_map;

extern int DEF_IMAGE_SIZE;
extern bool reverseKeyBoard;

extern int windowWidth;
extern int windowHeight;

extern int mapRows;
extern int mapColumns;

extern bool isServer;
extern char gameName[];

extern char *characterNameHost;
char *characterNameServer;

BattlegroundDynamic_element *mainCharacter;
extern GtkWidget *windowMain;

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
                gtk_fixed_put(GTK_FIXED(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
            // gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
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
                gtk_fixed_put(GTK_FIXED(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
            //gtk_layout_put(GTK_LAYOUT(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
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
            if (isServer)
            {
                mainCharacter = character[i];
            }
        }
        else
        {
            character[i]->image = gtk_image_new_from_file(characterImagePathHost);
            if (!isServer)
            {
                mainCharacter = character[i];
            }
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
            //gtk_layout_put(GTK_LAYOUT(lay), dynamic_objects_on_map->tabOfElements[a]->image, dynamic_objects_on_map->tabOfElements[a]->posX, dynamic_objects_on_map->tabOfElements[a]->posY);
            gtk_fixed_put(GTK_FIXED(lay), dynamic_objects_on_map->tabOfElements[a]->image, dynamic_objects_on_map->tabOfElements[a]->posX, dynamic_objects_on_map->tabOfElements[a]->posY);
            dynamic_objects_on_map->tabOfElements[a]->layout = lay;
        }
    }
}

void create_battleground(char mapPath[])
{
    //if (windowMain == NULL)
    //  return;
    printf("Początek\n");
    gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);

    //GtkWidget *window = window_creator_create_window();
    //GtkWidget *windowMain = window_creator_create_window();

    gtk_window_set_default_size(GTK_WINDOW(windowMain), windowWidth, windowHeight);
    gtk_window_set_resizable(GTK_WINDOW(windowMain), FALSE);
    gtk_window_set_position(GTK_WINDOW(windowMain), GTK_WIN_POS_CENTER);

    char *name;

    if (isServer == true)
        name = FullName_Path_get(gameName, characterNameServer);
    else
        name = FullName_Path_get(gameName, characterNameHost);

    gtk_window_set_title(GTK_WINDOW(windowMain), name);
    printf("Dodano tytuł\n");

    GtkAdjustment *hadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    GtkAdjustment *vadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    g_print("%f\n", gtk_adjustment_get_page_size(hadjCharacter));
    GtkWidget *view = gtk_viewport_new(hadjCharacter, vadjCharacter);

    GtkWidget *lay = gtk_fixed_new();
    //gtk_layout_new(hadj, vadj);
    printf("Stworzono layout\n");

    //gtk_layout_set_size(GTK_LAYOUT(lay), mapRows * DEF_IMAGE_SIZE, mapColumns * DEF_IMAGE_SIZE);
    //gtk_layout_set_size(GTK_LAYOUT(lay), windowWidth, windowHeight);
    GtkWidget *scrollW = gtk_scrolled_window_new(NULL, NULL);
    //gtk_scrolled_window_set_overlay_scrolling(,TRUE);
    gtk_widget_hide(GTK_WIDGET(gtk_scrolled_window_get_vscrollbar (GTK_SCROLLED_WINDOW(scrollW))));
    gtk_widget_hide(GTK_WIDGET(gtk_scrolled_window_get_hscrollbar (GTK_SCROLLED_WINDOW(scrollW))));

    gtk_container_add(GTK_CONTAINER(view), lay);
    gtk_container_add(GTK_CONTAINER(scrollW),view);
    gtk_container_add(GTK_CONTAINER(windowMain), scrollW);

    Prototype_map *pr_map = prototype_load_map(mapPath);

    set_characters_index(pr_map);

    create_battleground_static(windowMain, pr_map, lay);

    create_battleground_dynamic(windowMain, pr_map, lay);

    create_battleground__static_top(windowMain, pr_map, lay);

    CharacterData *characData = (CharacterData *)(mainCharacter->objectData);
    characData->hadj = gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(view));
    characData->vadj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(view));

     set_view_center_By_Character((void *)mainCharacter);

    //sterowanie postacią
    gtk_widget_add_events(windowMain, GDK_KEY_PRESS_MASK);

    if (reverseKeyBoard == true)
    {
        g_signal_connect(G_OBJECT(windowMain), "key_press_event", G_CALLBACK(objects_movie_Key_Reverse), mainCharacter);
    }
    else
    {
        g_signal_connect(G_OBJECT(windowMain), "key_press_event", G_CALLBACK(objects_movie), mainCharacter);
    }

    gtk_widget_show_all(windowMain);
    //printf("All correct\n");
   // gtk_main();
}
