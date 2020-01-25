#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "Objects_events.h"
#include "Object_gate_events.h"
#include "Objects_events_synchronization.h"
#include "Object_key_events.h"
#include "Object_trap_events.h"
#include "Object_character_events.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"


extern BattlegroundStatic *static_objects_on_map;
extern int DEF_IMAGE_SIZE;
extern int defaultCharTabLength;

bool isHumanoidOnPath(BattlegroundDynamic_element *object, int X, int Y)
{
    if (strcmp("path", static_objects_on_map->map[(Y / DEF_IMAGE_SIZE) * static_objects_on_map->X + X / DEF_IMAGE_SIZE]->type))
        return true;

    for (int i = 0; i < 8; i++)
    {
        if (strcmp("path", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y)) / DEF_IMAGE_SIZE) * static_objects_on_map->X + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX) + X) / DEF_IMAGE_SIZE)]->type))
        {
            return true;
        }
    }


    return false;
}

bool isColisionDynamic(BattlegroundDynamic_element *character_or_monster, BattlegroundDynamic_element *obj)
{

    if (strcmp("character", character_or_monster->type) && strcmp("monster", character_or_monster->type))
    {
        // printf(u8"Error! Niepoprawny typ, parametr1 funkcja isColisionDynamic\n");
        return false;
    }

    if (!strcmp("character", obj->type) || !strcmp("monster", obj->type))
    {
        int a = abs((character_or_monster->posX + character_or_monster->pivotPosX) - (obj->posX + obj->pivotPosX));
        int b = abs((character_or_monster->posY + character_or_monster->pivotPosY) - (obj->posY + obj->pivotPosY));

        double c = sqrt(a * a + b * b);
        if (c > character_or_monster->colisionCheckX + obj->colisionCheckX)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {

        for (int i = 0; i < 8; i++)
        {
            int y = (((sin((2.0 / 8.0 * i * 3.14))) * character_or_monster->colisionCheckY + character_or_monster->posY + character_or_monster->pivotPosY));
            int x = (((cos(2.0 / 8.0 * i * 3.14) * character_or_monster->colisionCheckX + character_or_monster->posX + character_or_monster->pivotPosX)));

            if ((obj->posX + obj->pivotPosX + obj->colisionCheckX) >= (x) && (obj->posX + obj->pivotPosX - obj->colisionCheckX) <= (x) && (obj->posY + obj->pivotPosY + obj->colisionCheckY) >= (y) && (obj->posY + obj->pivotPosY - obj->colisionCheckY) <= (y))
            {
                return true;
            }
        }
    }

    return false;
}


bool isCharacterInRangeOfAction(BattlegroundDynamic_element *character, BattlegroundDynamic_element *obj)
{
    for (int i = 0; i < 8; i++)
    {

        int y = (((sin((2.0 / 8.0 * i * 3.14))) * character->actionRange + character->posY + character->pivotPosY));
        int x = (((cos(2.0 / 8.0 * i * 3.14) * character->actionRange + character->posX + character->pivotPosX)));

        if ((obj->posX + obj->pivotPosX + obj->colisionCheckX) >= (x) && (obj->posX + obj->pivotPosX - obj->colisionCheckX) <= (x) && (obj->posY + obj->pivotPosY + obj->colisionCheckY) >= (y) && (obj->posY + obj->pivotPosY - obj->colisionCheckY) <= (y))
            return true;
    }

    return false;
}

void make_move(BattlegroundDynamic_element *object, int oX, int oY)
{
    if (object == NULL)
        return;

    int x = object->posX + oX + object->pivotPosX;
    int y = object->posY + oY + object->pivotPosY;

    if (!strcmp(object->type, "character") || !strcmp(object->type, "monster"))
    {
        if (isHumanoidOnPath(object, x, y) == false && humanoidColisionGate(object, x, y) == false)
        {
            object->posX = object->posX + oX;
            object->posY = object->posY + oY;
            if (object->image != NULL)
            {
                //gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
                char action[defaultCharTabLength];
                sprintf(action, "move-%i-%i", object->posY, object->posX);
                newSmallSynchronizationEvent(object, action);
                gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
            }
        }
        else
        {
            if (!strcmp(object->type, "monster"))
            {
                MonsterData *mData = (MonsterData *)object->objectData;
                mData->isColision = true;
            }
        }

        if (!strcmp(object->type, "character"))
        {
            characterGetKey(object);

            characterStepOnTrap(object);
            characterSavePrinces(object);
        }
    }
}




