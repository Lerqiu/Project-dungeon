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
extern char *mapPath;

extern char *characterNameHost;
char *characterNameServer;

extern int MainCharacterServerSearchMapIndex;

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
                gtk_fixed_put(GTK_FIXED(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
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

    extern int characterHostIndexX;
    extern int characterHostIndexY;
    extern char *characterImagePathHost;

    extern int MainCharacterServerSearchMapIndex;

    character[MainCharacterServerSearchMapIndex]->image = gtk_image_new_from_file(characterImagePathServer);
    character[1 - MainCharacterServerSearchMapIndex]->image = gtk_image_new_from_file(characterImagePathHost);

    if (isServer)
    {
        mainCharacter = character[MainCharacterServerSearchMapIndex];
        characterServerIndexX = character[MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterServerIndexY = character[MainCharacterServerSearchMapIndex]->indexStartPointY;

        characterHostIndexX = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterHostIndexY = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointY;
    }
    else
    {
        mainCharacter = character[1 - MainCharacterServerSearchMapIndex];
        characterServerIndexX = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterServerIndexY = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointY;

        characterHostIndexX = character[MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterHostIndexY = character[MainCharacterServerSearchMapIndex]->indexStartPointY;
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
            gtk_fixed_put(GTK_FIXED(lay), dynamic_objects_on_map->tabOfElements[a]->image, dynamic_objects_on_map->tabOfElements[a]->posX, dynamic_objects_on_map->tabOfElements[a]->posY);
            dynamic_objects_on_map->tabOfElements[a]->layout = lay;
        }
    }
}

GtkWidget *labelsTabRightPanel[4];
void createRightPanel(GtkWidget *MainBox)
{
    GtkWidget *MRBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(MRBox, FALSE);
    gtk_container_add(GTK_CONTAINER(MainBox), MRBox);

    char rightPanelTabChar[4][2][500] = {
        {"Images/Dynamic/key.png", "0"},
        {"Images/Dynamic/key.png", "0"},
        {"Images/Dynamic/key.png", "0"},
        {"Images/Dynamic/key.png", "0"}};

    for (int i = 0; i < 4; i++)
    {
        GtkWidget *boxK = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *imageK = gtk_image_new_from_file(rightPanelTabChar[i][0]);
        labelsTabRightPanel[i] = gtk_label_new(rightPanelTabChar[i][1]);
        gtk_container_add(GTK_CONTAINER(boxK), imageK);
        gtk_container_add(GTK_CONTAINER(boxK), labelsTabRightPanel[i]);
        gtk_container_add(GTK_CONTAINER(MRBox), boxK);
    }
}

void changeBackgroundCollor(GtkWidget *window)
{
    GtkCssProvider *cssP = gtk_css_provider_new();
    GdkDisplay *dis = gdk_display_get_default();
    GdkScreen *scr = gdk_display_get_default_screen(dis);

    gtk_style_context_add_provider_for_screen(scr, GTK_STYLE_PROVIDER(cssP), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(cssP),
                                    "window {  \n"
                                    "   background-color: #201d20;    \n"
                                    //"   background-color: red;    \n"
                                    "} \n",
                                    -1, NULL);
    g_object_unref(cssP);
}

extern GtkWidget *windowMain;
void create_battleground()
{
    gtk_window_set_default_size(GTK_WINDOW(windowMain), windowWidth, windowHeight);
    gtk_window_set_resizable(GTK_WINDOW(windowMain), FALSE);
    gtk_window_set_position(GTK_WINDOW(windowMain), GTK_WIN_POS_CENTER);
    changeBackgroundCollor(windowMain);

    char *name;
    if (isServer == true)
        name = FullName_Path_get(gameName, characterNameServer);
    else
        name = FullName_Path_get(gameName, characterNameHost);

    gtk_window_set_title(GTK_WINDOW(windowMain), name);

    GtkAdjustment *hadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    GtkAdjustment *vadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);

    /*
    GtkWidget *lay = gtk_fixed_new(hadjCharacter, vadjCharacter);
    gtk_container_add(GTK_CONTAINER(windowMain), lay);
    */
    Prototype_map *pr_map = prototype_load_map(mapPath);

    GtkWidget *lay = gtk_fixed_new();
    GtkWidget *view = gtk_viewport_new(hadjCharacter, vadjCharacter);
    GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(view), lay);
    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_container_add(GTK_CONTAINER(mainBox), scroll);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(scroll)));
    gtk_widget_hide(gtk_scrolled_window_get_hscrollbar(GTK_SCROLLED_WINDOW(scroll)));

    createRightPanel(mainBox);
    gtk_container_add(GTK_CONTAINER(windowMain), mainBox);

    create_battleground_static(windowMain, pr_map, lay);

    create_battleground_dynamic(windowMain, pr_map, lay);

    create_battleground__static_top(windowMain, pr_map, lay);

    CharacterData *characData = (CharacterData *)(mainCharacter->objectData);
    characData->hadj = gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(view));
    characData->vadj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(view));

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

    set_view_center_By_Character((void *)mainCharacter);

    gtk_widget_show_all(windowMain);
    g_timeout_add(1000 / 60, readSynchronizationEvent, NULL);
}
