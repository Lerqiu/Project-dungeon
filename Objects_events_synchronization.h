#ifndef Objects_events_synchronization
#define Objects_events_synchronization

#include <gtk/gtk.h>

gboolean readSynchronizationEvent(gpointer data);
void newSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[]);
void newSmallSynchronizationEvent(void *el, char event[]);

void deactive_newSynchronizationEvent(void);

#endif