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

void characterGetKey(BattlegroundDynamic_element *object)
{
    if (object == NULL)
        return;

    BattlegroundDynamic *keys = getObjectByType("key");

    if (keys->amount < 1)
        return;

    for (int i = 0; i < keys->amount; i++)
    {
        if (isColisionDynamic(object, keys->tabOfElements[i]))
        {
            KeyData *k = (KeyData *)keys->tabOfElements[i]->objectData;
            CharacterData *ch = (CharacterData *)object->objectData;

            if (k->key_type < 4 && k->key_type >= 0)
            {
                ch->keyTab[k->key_type]++;

                extern GtkWidget *labelsTabRightPanel[4];
                char buffer[100];
                sprintf(buffer, "%i", ch->keyTab[k->key_type]);
                gtk_label_set_text(GTK_LABEL(labelsTabRightPanel[k->key_type]), buffer);

                newSmallSynchronizationEvent(keys->tabOfElements[i], "vanish");
                return;
            }
        }
    }
}

void characterKeyUse(BattlegroundDynamic_element *object, int key)
{
    if (object == NULL)
        return;
    if (object->objectData == NULL)
        return;

    CharacterData *ch = (CharacterData *)object->objectData;
    if (key >= 4 || key < 0)
        return;
    if (ch->keyTab[key] <= 0)
        return;

    BattlegroundDynamic *gates = getObjectByType("gate");

    if (gates->amount < 1)
        return;

    for (int i = 0; i < gates->amount; i++)
    {
        if (isCharacterInRangeOfAction(object, gates->tabOfElements[i]))
        {
            if (gates->tabOfElements[i] == NULL)
                continue;
            GateData *g = (GateData *)gates->tabOfElements[i]->objectData;
            if (g->key_type == key && !g->isOpen)
            {
                ch->keyTab[key]--;
                extern GtkWidget *labelsTabRightPanel[4];
                char buffer[100];
                sprintf(buffer, "%i", ch->keyTab[key]);
                gtk_label_set_text(GTK_LABEL(labelsTabRightPanel[key]), buffer);

                newSmallSynchronizationEvent(gates->tabOfElements[i], "open");
                return;
            }
        }
    }
}

void Synchronization_key_vanish(int indexY, int indexX)
{
    delete_BattlegroundDynamic_element(indexY, indexX);
}