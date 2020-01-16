#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "settings.h"
#include "fifo.h"

#include "battleground.h"
#include "objects_events.h"

extern BattlegroundDynamic *dynamic_objects_on_map;

void make_move_Synchronization(int indexY, int indexX, char event[])
{
   
    BattlegroundDynamic_element *object = NULL;
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if(dynamic_objects_on_map->tabOfElements[i]!=NULL)
        if (dynamic_objects_on_map->tabOfElements[i]->indexStartPointY == indexY && dynamic_objects_on_map->tabOfElements[i]->indexStartPointX == indexX)
        {
            object = dynamic_objects_on_map->tabOfElements[i];
            break;
        }
    }
    if (object == NULL)
        return;

    int X, Y;
    if (sscanf(event, "move-%i-%i", &Y, &X) < 2)
        return;

    int x = X;
    int y = Y;

    if (object->image != NULL)
    {
        gtk_layout_move(GTK_LAYOUT(object->layout), object->image, X, Y);
        //printf("move-%i-%i\n",Y,X);
    }
}