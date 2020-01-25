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

#endif