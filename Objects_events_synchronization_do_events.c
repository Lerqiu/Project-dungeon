#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "Default_settings.h"
#include "Windows_FIFO.h"
#include "Objects_events.h"


void make_move_Synchronization(Pointer_and_Index *poi, char event[])
{
    int X, Y;
    if (sscanf(event, "move-%i-%i", &Y, &X) < 2)
        return;

    if (poi == NULL)
        return;

    BattlegroundDynamic_element *object = poi->pointer;

    int x = X;
    int y = Y;

    if (object->image != NULL)
    {
        gtk_fixed_move(GTK_FIXED(object->layout), object->image, X, Y);
        //printf("move-%i-%i\n",Y,X);
    }
    free(poi);
}
