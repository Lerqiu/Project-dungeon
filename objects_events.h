#ifndef objects_events
#define objects_events

#include <gtk/gtk.h>
#include "battleground.h"

void objects_movie_up(gpointer *pointer);
void objects_movie_right(gpointer *pointer);
void objects_movie_down(gpointer *pointer);
void objects_movie_left(gpointer *pointer);

void set_view_center_By_Character(void *ob);

gboolean objects_movie_Key_Reverse(GtkWidget *widget, GdkEventKey *event, gpointer *date);
gboolean objects_movie(GtkWidget *widget, GdkEventKey *event, gpointer *date);

gboolean readSynchronizationEvent(gpointer data);
void newSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[]);
void newSmallSynchronizationEvent(void *elem, char event[]);

typedef struct
{
    int index;
    BattlegroundDynamic_element *pointer;
} Pointer_and_Index;

Pointer_and_Index *getObject_by_ids(int indexY, int intexX);
void make_move_Synchronization(Pointer_and_Index *object, char event[]);
void open_gate_Synchronization(Pointer_and_Index *object, char event[]);
void key_vanish_Synchronization(Pointer_and_Index *poi, char event[]);

#endif