#ifndef Menu_windows_functionalities
#define Menu_windows_functionalities

#include <gtk/gtk.h>

typedef struct checkCorrectData_struct
{
    GtkWidget *window;
    GtkWidget *entryNick;
    GtkWidget *switchSer_Hos;
    GtkWidget *switchKey;
    GSList *radioButtons;
} CheckCorrectData_Struct;


void destroyStartWindowContainers(void);
void checkCorrectData(GtkWidget *PlayButton, gpointer data);
gboolean createStartWindowShowHideMaps(GtkWidget *widget, GParamSpec *pspec, gpointer data);
GSList *createStartWindowShowMap(GtkWidget *parentContainer);

void destroyWindow(gpointer data);
void changeBackgroundCollor(GtkWidget *window,char color[]);
gboolean enter_escape_key_events(GtkWidget *widget, GdkEventKey *event, gpointer data);

#endif