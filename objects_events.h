#ifndef objects_events
#define objects_events

#include <gtk/gtk.h>

void objects_movie_up(gpointer *pointer);
void objects_movie_right(gpointer *pointer);
void objects_movie_down(gpointer *pointer);
void objects_movie_left(gpointer *pointer);

void set_view_center_By_Character(void *ob);

gboolean objects_movie_Key_Reverse(GtkWidget *widget,GdkEventKey *event,gpointer *date);
gboolean objects_movie(GtkWidget *widget,GdkEventKey *event,gpointer *date);

gboolean readSynchronizationEvent(gpointer data);
void newSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[]);
void newSmallSynchronizationEvent(void *elem, char event[]);

void make_move_Synchronization(int indexY, int indexX, char event[]);

#endif