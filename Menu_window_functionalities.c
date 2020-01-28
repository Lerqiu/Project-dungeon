#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#include "Menu_window_functionalities.h"
#include "Menu_window_view.h"
#include "Set_connections_betwen_players.h"
#include "Default_settings.h"
#include "Windows_FIFO.h"
#include "Objects_basic_types.h"

extern char gameName[];
extern int maxLengthOfPath;
extern char folderPathMaps[];
extern char folderPathOthers[];
extern BattlegroundDynamic *dynamic_objects_on_map;

GSList *createStartWindowShowMap(GtkWidget *parentContainer)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(folderPathMaps);
    GSList *glist = NULL;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] != '.')
            {

                GtkWidget *w = gtk_radio_button_new_with_label(glist, dir->d_name);
                glist = gtk_radio_button_get_group(GTK_RADIO_BUTTON(w));
                gtk_container_add(GTK_CONTAINER(parentContainer), w);
                gtk_widget_show(w);
                gtk_widget_set_hexpand(w, TRUE);
                gtk_widget_set_halign(w, GTK_ALIGN_CENTER);
            }
        }
        closedir(d);
    }
    gtk_widget_hide(parentContainer);
    return glist;
}

gboolean createStartWindowShowHideMaps(GtkWidget *widget, GParamSpec *pspec, gpointer data)
{
    GtkWidget *p = (GtkWidget *)(data);
    if (gtk_switch_get_active(GTK_SWITCH(widget)))
    {
        gtk_widget_show(p);
    }
    else
    {
        gtk_widget_hide(p);
    }

    return TRUE;
}

static void showError(char text[], CheckCorrectData_Struct *data)
{
    GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(data->window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s%s", "Niepoprawny typ danych:", text);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

void checkCorrectData(GtkWidget *PlayButton, gpointer data)
{
    if (isCheckingPipe())
        return;
    char *Nick;
    bool isServer;
    bool isKeyRevert;
    char *Map = NULL;

    CheckCorrectData_Struct *checkStructData = (CheckCorrectData_Struct *)data;
    if (checkStructData->entryNick == NULL)
        return;
    const gchar *n = gtk_entry_get_text(GTK_ENTRY(checkStructData->entryNick));
    Nick = (char *)malloc(sizeof(char) * (strlen(n) + 1));
    strcpy(Nick, n);

    if (strlen(Nick) < 3)
    {
        showError("Za krótki nick (minimum 3 znaki)", checkStructData);
        return;
    }
    for (int i = 0; i < (int)(strlen(Nick)); i++)
    {
        if (Nick[i] == ' ' || Nick[i] == '\t')
        {
            showError("Białe znaki w nicku", checkStructData);
            return;
        }
    }

    if (gtk_switch_get_active(GTK_SWITCH(checkStructData->switchSer_Hos)) == TRUE)
    {
        isServer = true;
    }
    else
    {
        isServer = false;
    }

    if (gtk_switch_get_active(GTK_SWITCH(checkStructData->switchKey)) == TRUE)
    {
        isKeyRevert = true;
    }
    else
    {
        isKeyRevert = false;
    }

    GSList *li = checkStructData->radioButtons;
    while (li)
    {
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(li->data)) == TRUE)
        {
            const gchar *t = gtk_button_get_label(GTK_BUTTON(li->data));
            Map = (char *)malloc(sizeof(char) * (strlen(t) + 1));
            strcpy(Map, t);
        }
        li = li->next;
    }

    set_connection(Nick, isServer, isKeyRevert, Map, checkStructData->window);
}

extern GtkWidget *windowMain;
extern GtkWidget *scrollWindowWidget;

void destroyStartWindowContainers(void)
{
    if (scrollWindowWidget != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(windowMain), scrollWindowWidget);
        if (GTK_IS_WIDGET(scrollWindowWidget))
            gtk_widget_destroy(scrollWindowWidget);
        scrollWindowWidget = NULL;
    }
}

void destroyWindow(gpointer data)
{
    if (isConnectedPipe())
    {
        sendStringToPipe("Synchronization GtkWindow 0 0 destroy\n");
    }

    gtk_main_quit();
}

gboolean enter_escape_key_events(GtkWidget *widget, GdkEventKey *event, gpointer data)
{

    if (event->keyval == GDK_KEY_Escape)
    {
        gtk_widget_destroy(windowMain);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Return)
    {
        if (dynamic_objects_on_map == NULL)
            checkCorrectData(NULL, data);
        return TRUE;
    }

    return FALSE;
}