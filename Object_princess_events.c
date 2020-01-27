#include <stdbool.h>

#include "Objects_events.h"
#include "Objects_basic_types.h"
#include "Objects_events_synchronization.h"
#include "Default_settings.h"

extern int DEF_IMAGE_SIZE;

bool princes_save(BattlegroundDynamic_element *object)
{
    if (object == NULL)
        return false;
    if (strcmp(object->type, "character"))
        return false;

    BattlegroundDynamic *princess = getObjectByType("princess");

    if (princess->amount < 1)
        return false;

    for (int i = 0; i < princess->amount; i++)
    {
        //Uratowanie księżniczki

        if (princess->tabOfElements[i]->indexStartPointX == (object->posX + object->pivotPosX) / DEF_IMAGE_SIZE &&
            princess->tabOfElements[i]->indexStartPointY == (object->posY + object->pivotPosY) / DEF_IMAGE_SIZE)
            newSmallSynchronizationEvent(object, "princessSaved");

        return true;
    }

    return false;
}