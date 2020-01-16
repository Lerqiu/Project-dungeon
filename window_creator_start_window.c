#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#include "window_creator.h"
#include "settings.h"
#include "connection.h"
#include "fifo.h"

extern char gameName[];
extern int maxLengthOfPath;
extern char folderPathMaps[];
extern char folderPathOthers[];

//Okienko startowe

static GSList *createStartWindowShowMap(GtkWidget *parentContainer)
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

static gboolean createStartWindowShowHideMaps(GtkWidget *widget, GParamSpec *pspec, gpointer data)
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

typedef struct checkCorrectData_struct
{
    GtkWidget *window;
    GtkWidget *entryNick;
    GtkWidget *switchSer_Hos;
    GtkWidget *switchKey;
    GSList *radioButtons;
} CheckCorrectData_Struct;

static void showError(char text[], CheckCorrectData_Struct *data)
{
    GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(data->window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s%s", "Niepoprawny typ danych:", text);
    gtk_dialog_run(GTK_DIALOG(d));
    gtk_widget_destroy(d);
}

static void checkCorrectData(GtkWidget *PlayButton, gpointer data)
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

GtkWidget *windowMain = NULL;
static GtkWidget *scrollWindowWidget = NULL;

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

static void destroyWindow()
{
    closePipes();
    gtk_main_quit();
}

void createStartWindow(void)
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
    gtk_main();
}
