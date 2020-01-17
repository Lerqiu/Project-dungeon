#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "settings.h"
#include "fifo.h"

#include "battleground.h"
#include "objects_events.h"

extern int maxLengthOfPath;

void recivedSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[]);

void newSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[])
{
    char buffer[maxLengthOfPath * 4];
    sprintf(buffer, "Synchronization %s %i %i %s\n", typeOfObject, indexY, indexX, event);
    sendStringToPipe(buffer);

    recivedSynchronizationEvent(typeOfObject, indexY, indexX, event);
    //printf("Synchronization: OType:%s IY:%i IX:%i Event:%s\n",typeOfObject,indexY,indexX,event);
}

void newSmallSynchronizationEvent(void *el, char event[])
{
    BattlegroundDynamic_element *element = (BattlegroundDynamic_element *)el;
    char typeOfObject[maxLengthOfPath];
    strcpy(typeOfObject, element->type);
    int indexY = element->indexStartPointY;
    int indexX = element->indexStartPointX;

    newSynchronizationEvent(typeOfObject, indexY, indexX, event);
}

gboolean readSynchronizationEvent(gpointer data)
{
    char buffer[maxLengthOfPath * 4];
    if (!getStringFromPipe(buffer, maxLengthOfPath * 4))
        return TRUE;

    char typeOfObject[maxLengthOfPath];
    int indexY;
    int indexX;
    char event[maxLengthOfPath];

    if (sscanf(buffer, "Synchronization %s %i %i %s", typeOfObject, &indexY, &indexX, event) < 4)
        return TRUE;

    recivedSynchronizationEvent(typeOfObject, indexY, indexX, event);

    //printf("Synchronization: OType:%s IY:%i IX:%i Event:%s\n",typeOfObject,indexY,indexX,event);
    readSynchronizationEvent(NULL); //Wymagane by zachować płynność gry
    return TRUE;
}

void recivedSynchronizationEvent(char typeOfObject[], int indexY, int indexX, char event[])
{
    if (!strcmp("character", typeOfObject) || !strcmp("monster", typeOfObject))
    {
        if (strlen(event) < 4)
            return;
        char action[maxLengthOfPath];
        memcpy(action, event, 4);
        action[4] = '\0';
        //printf("momo:%s\n", action);
        if (!strcmp(action, "move"))
        {
            //printf("redy for movie\n");
            make_move_Synchronization(getObject_by_ids(indexY, indexX), event);
        }
    }
    else if (!strcmp("gate", typeOfObject))
    {
        if (!strcmp("open", event))
        {
            open_gate_Synchronization(getObject_by_ids(indexY, indexX), event);
        }
    }
    else if (!strcmp("key", typeOfObject))
    {
        if (!strcmp("vanish", event))
        {
            key_vanish_Synchronization(getObject_by_ids(indexY, indexX), event);
        }
    }
}