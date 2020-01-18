#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "settings.h"
#include "fifo.h"

#include "battleground.h"
#include "objects_events.h"

extern BattlegroundDynamic *dynamic_objects_on_map;

Pointer_and_Index *getObject_by_ids(int indexY, int indexX)
{
    BattlegroundDynamic_element *object = NULL;
    int index;
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (dynamic_objects_on_map->tabOfElements[i]->indexStartPointY == indexY && dynamic_objects_on_map->tabOfElements[i]->indexStartPointX == indexX)
            {
                object = dynamic_objects_on_map->tabOfElements[i];
                index = i;
                break;
            }
    }
    if (object == NULL)
        return NULL;

    Pointer_and_Index *poi = (Pointer_and_Index *)malloc(sizeof(Pointer_and_Index));
    poi->pointer = object;
    poi->index = index;
    return poi;
}

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

void open_gate_Synchronization(Pointer_and_Index *poi, char event[])
{
    if (poi == NULL)
        return;

    BattlegroundDynamic_element *object = poi->pointer;
    gtk_image_clear(GTK_IMAGE(object->image));
    gtk_container_remove(GTK_CONTAINER(object->layout), object->image);
    object->image = NULL;

    free(object);
    dynamic_objects_on_map->tabOfElements[poi->index] = NULL;
    free(poi);
}

void key_vanish_Synchronization(Pointer_and_Index *poi, char event[])
{
    if (poi == NULL)
        return;

    BattlegroundDynamic_element *object = poi->pointer;
    gtk_image_clear(GTK_IMAGE(object->image));
    gtk_container_remove(GTK_CONTAINER(object->layout), object->image);
    object->image = NULL;

    free(object);
    dynamic_objects_on_map->tabOfElements[poi->index] = NULL;

    free(poi);
}