#include <stdio.h>
#include <stdlib.h>

#include "Objects_basic_types.h"
#include <string.h>

BattlegroundStatic *static_objects_on_map = NULL;
BattlegroundDynamic *dynamic_objects_on_map = NULL;

BattlegroundDynamic_element *mainCharacter = NULL;
BattlegroundDynamic_element *showCharacterPointer = NULL;

BattlegroundDynamic *getObjectByType(char type[])
{

    BattlegroundDynamic *objects;
    int amount = 0;

    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, type))
            {
                amount++;
            }
    }

    objects = (BattlegroundDynamic *)malloc(sizeof(BattlegroundDynamic));
    objects->amount = amount;
    objects->tabOfElements = (BattlegroundDynamic_element **)malloc(sizeof(BattlegroundDynamic_element *) * (objects->amount));

    amount = 0;
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, type))
            {
                objects->tabOfElements[amount] = dynamic_objects_on_map->tabOfElements[i];

                amount++;
            }
    }

    return objects;
}

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

void delete_BattlegroundDynamic_element(int indexY, int indexX)
{
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (dynamic_objects_on_map->tabOfElements[i]->indexStartPointY == indexY && dynamic_objects_on_map->tabOfElements[i]->indexStartPointX == indexX)
            {

                gtk_image_clear(GTK_IMAGE(dynamic_objects_on_map->tabOfElements[i]->image));
                gtk_container_remove(GTK_CONTAINER(dynamic_objects_on_map->tabOfElements[i]->layout), dynamic_objects_on_map->tabOfElements[i]->image);
                dynamic_objects_on_map->tabOfElements[i]->image = NULL;

                free(dynamic_objects_on_map->tabOfElements[i]->objectData);
                dynamic_objects_on_map->tabOfElements[i]->objectData = NULL;
                free(dynamic_objects_on_map->tabOfElements[i]);
                dynamic_objects_on_map->tabOfElements[i] = NULL;
                return;
            }
    }
}