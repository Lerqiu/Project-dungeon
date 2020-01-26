#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "Default_settings.h"
#include "Objects_events.h"
#include "Objects_basic_types.h"

bool humanoidColisionGate(BattlegroundDynamic_element *object, int oX, int oY)
{
    int x, y;
    x = object->posX;
    y = object->posY;
    object->posX = oX - object->pivotPosX;
    object->posY = oY - object->pivotPosY;

    BattlegroundDynamic *gates = getObjectByType("gate");

    if (gates->amount < 1)
        return false;

    for (int i = 0; i < gates->amount; i++)
    {
        GateData *g = (GateData *)gates->tabOfElements[i]->objectData;

        if (g->isOpen == false && isColisionDynamic(object, gates->tabOfElements[i]))
        {
            object->posX = x;
            object->posY = y;
            return true;
        }
    }

    object->posX = x;
    object->posY = y;

    return false;
}

bool isThereAGate(int indexY, int indexX)
{

    BattlegroundDynamic *gates = getObjectByType("gate");

    for (int i = 0; i < gates->amount; i++)
    {

        if (indexY == gates->tabOfElements[i]->indexStartPointY && indexX == gates->tabOfElements[i]->indexStartPointX)
        {
            return true;
        }
    }

    return false;
}

void Synchronization_open_gate(int indexY, int indexX)
{
    delete_BattlegroundDynamic_element(indexY, indexX);
}
