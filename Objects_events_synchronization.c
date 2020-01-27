#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "Default_settings.h"
#include "Windows_FIFO.h"
#include "Menu_window_functionalities.h"

#include "Objects_basic_types.h"
#include "Objects_events_synchronization.h"

#include "Object_gate_events.h"
#include "Object_key_events.h"
#include "Object_trap_events.h"
#include "Object_monster_events.h"
#include "Object_character_events.h"

extern int defaultCharTabLength;

void recivedSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[]);

bool isNewSynchronizationActive = true;

void deactive_newSynchronizationEvent(void)
{
    isNewSynchronizationActive = false;
}

void newSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[])
{
    if (!isNewSynchronizationActive)
        return;

    char buffer[defaultCharTabLength * 4];
    sprintf(buffer, "Synchronization %s %i %i %s\n", typeOfObject, indexY, indexX, event);
    sendStringToPipe(buffer);

    recivedSynchronizationEvent(typeOfObject, indexY, indexX, event);
}

void newSmallSynchronizationEvent(void *el, char event[])
{
    BattlegroundDynamic_element *element = (BattlegroundDynamic_element *)el;
    char typeOfObject[defaultCharTabLength];
    strcpy(typeOfObject, element->type);
    int indexY = element->indexStartPointY;
    int indexX = element->indexStartPointX;

    newSynchronizationEvent(typeOfObject, indexY, indexX, event);
}

gboolean readSynchronizationEvent(gpointer data)
{
    char buffer[defaultCharTabLength * 4];
    if (!getStringFromPipe(buffer, defaultCharTabLength * 4))
        return TRUE;

    char typeOfObject[defaultCharTabLength];
    int indexY;
    int indexX;
    char event[defaultCharTabLength];

    if (sscanf(buffer, "Synchronization %s %i %i %s", typeOfObject, &indexY, &indexX, event) < 4)
        return TRUE;

    recivedSynchronizationEvent(typeOfObject, indexY, indexX, event);

    readSynchronizationEvent(NULL); //Wymagane by zachować płynność gry
    return TRUE;
}

void recivedSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[])
{
    if (!strcmp("character", typeOfObject))
    {
        if (strlen(event) < 4)
            return;
        char action[defaultCharTabLength];
        memcpy(action, event, 4);
        action[4] = '\0';
        if (!strcmp(action, "move"))
        {
            Synchronization_move_character(getObject_by_ids(indexY, indexX), event);
        }
        else if (!strcmp(event, "dead"))
        {
            Pointer_and_Index *poi = getObject_by_ids(indexY, indexX);
            if (poi != NULL)
                Synchronization_character_dead(poi->pointer);
        }
        else if (!strcmp(event, "princessSaved"))
        {
            Pointer_and_Index *poi = getObject_by_ids(indexY, indexX);
            if (poi != NULL)
                Synchronization_character_win(poi->pointer);
        }
    }
    else if (!strcmp("monster", typeOfObject))
    {
        if (strlen(event) < 4)
            return;
        char action[defaultCharTabLength];
        memcpy(action, event, 4);
        action[4] = '\0';
        if (!strcmp(action, "move"))
        {
            Synchronization_move_monster(getObject_by_ids(indexY, indexX), event);
        }
    }
    else if (!strcmp("gate", typeOfObject))
    {
        if (!strcmp("open", event))
        {
            Synchronization_open_gate(indexY, indexX);
        }
    }
    else if (!strcmp("key", typeOfObject))
    {
        if (!strcmp("vanish", event))
        {
            Synchronization_key_vanish(indexY, indexX);
        }
    }
    else if (!strcmp("trap", typeOfObject))
    {
        if (!strcmp("launched", event))
        {
            Synchronization_launched_trap(indexY, indexX);
        }
    }
    else if (!strcmp("GtkWindow", typeOfObject))
    {
        if (!strcmp("destroy", event))
        {
            destroyWindow(NULL);
        }
    }
    else
    {
        printf("Nieznany typ: %s %i %i %s\n", typeOfObject, indexY, indexX, event);
    }
}