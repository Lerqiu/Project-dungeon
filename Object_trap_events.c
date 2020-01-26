#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "Objects_events.h"
#include "Objects_events_synchronization.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

bool characterStepOnTrap(BattlegroundDynamic_element *object)
{

    BattlegroundDynamic *traps = getObjectByType("trap");

    for (int i = 0; i < traps->amount; i++)
    {
        if (isColisionDynamic(object, traps->tabOfElements[i]))
        {
            //Wejście na pułapkę
            if (!strcmp(object->type, "character"))
            {
                newSmallSynchronizationEvent(object, "dead");
                newSmallSynchronizationEvent(traps->tabOfElements[i], "launched");
            }

            return true;
        }
    }
    return false;
}

void Synchronization_launched_trap(int indexY, int indexX)
{
    delete_BattlegroundDynamic_element(indexY, indexX);
}
