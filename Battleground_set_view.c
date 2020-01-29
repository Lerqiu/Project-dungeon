#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "Battleground_set_view.h"
#include "Battleground_add_object_to_window.h"

#include "Object_character_events.h"
#include "Object_monster_events.h"
#include "Objects_events_synchronization.h"

#include "Default_settings.h"
#include "Menu_window_functionalities.h"

#include "Map_loader_prototype.h"
#include "Map_loader_statics_elements.h"
#include "Map_loader_dynamic_elements.h"
#include "Objects_basic_types.h"
#include "Objects_events.h"

extern bool reverseKeyBoard;

extern int windowWidth;
extern int windowHeight;

extern bool isServer;
extern char gameName[];
extern char *mapPath;

extern char *characterNameHost;
extern char *characterNameServer;

extern BattlegroundDynamic_element *mainCharacter;

GtkWidget *labelsTabRightPanel[4];
void createRightPanel(GtkWidget *MainBox)
{
    GtkWidget *MRBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(MRBox, FALSE);
    gtk_container_add(GTK_CONTAINER(MainBox), MRBox);

    char rightPanelTabChar[4][2][500] = {
        {"Images/Dynamic/key_0.png", "0"},
        {"Images/Dynamic/key_1.png", "0"},
        {"Images/Dynamic/key_2.png", "0"},
        {"Images/Dynamic/key_3.png", "0"}};

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

extern GtkWidget *windowMain;

static gboolean closeGame(gpointer data)
{
    destroyWindow(NULL);
    return FALSE;
}

static gboolean hideMessage(gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(data));
    return FALSE;
}

void showEndInfoAboutGame(char text[], int time)
{

    GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(windowMain), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "%s", text);
    gtk_widget_show(d);

    if (time > 0)
        g_timeout_add(time, closeGame, NULL);

    if (time < 0)
        g_timeout_add(-1 * time, hideMessage, d);
}

void create_battleground()
{
    //gtk_window_set_default_size(GTK_WINDOW(windowMain), windowWidth, windowHeight);
    //gtk_window_set_resizable(GTK_WINDOW(windowMain), FALSE);
    gtk_widget_set_size_request(GTK_WIDGET(windowMain), windowWidth, windowHeight);
    gtk_window_set_position(GTK_WINDOW(windowMain), GTK_WIN_POS_CENTER);
    changeBackgroundCollor(windowMain,"#201d20");

    char *name;
    if (isServer == true)
        name = FullName_Path_get(gameName, characterNameServer);
    else
        name = FullName_Path_get(gameName, characterNameHost);

    gtk_window_set_title(GTK_WINDOW(windowMain), name);

    GtkAdjustment *hadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    GtkAdjustment *vadjCharacter = gtk_adjustment_new(0, 0, 0, 0, 0, 0);

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

    create_battleground_dynamic(windowMain, pr_map, lay, gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(view)), gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(view)));

    create_battleground__static_top(windowMain, pr_map, lay);

    //sterowanie postacią
    gtk_widget_add_events(windowMain, GDK_KEY_PRESS_MASK);

    if (reverseKeyBoard == true)
    {
        g_signal_connect(G_OBJECT(windowMain), "key_press_event", G_CALLBACK(character_movie_keyboard_reverse), mainCharacter);
    }
    else
    {
        g_signal_connect(G_OBJECT(windowMain), "key_press_event", G_CALLBACK(character_movie_keyboard), mainCharacter);
    }

    if (isServer)
    {
        g_timeout_add(1000 / 30, monster_move, NULL);
    }

    gtk_widget_show_all(windowMain);
    g_timeout_add(1000 / 30, readSynchronizationEvent, NULL);
    g_timeout_add(1000 / 60, set_view_center_object, NULL);
}