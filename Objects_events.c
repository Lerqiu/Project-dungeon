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



extern int DEF_IMAGE_SIZE;
extern int defaultCharTabLength;
extern int windowHeight;
extern int windowWidth;

extern BattlegroundStatic *static_objects_on_map;
extern BattlegroundDynamic_element *showCharacterPointer;

bool isA_object(char type[],int X, int Y)
{
    if (!strcmp(type, static_objects_on_map->map[(Y / DEF_IMAGE_SIZE) * static_objects_on_map->X + X / DEF_IMAGE_SIZE]->type))
        return true;

    return false;
}


bool isHumanoidOnPath(BattlegroundDynamic_element *object, int X, int Y)
{
    if (strcmp("path", static_objects_on_map->map[(Y / DEF_IMAGE_SIZE) * static_objects_on_map->X + X / DEF_IMAGE_SIZE]->type))
        return false;

    for (int i = 0; i < 8; i++)
    {
        if (strcmp("path", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y)) / DEF_IMAGE_SIZE) * static_objects_on_map->X + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX) + X) / DEF_IMAGE_SIZE)]->type))
        {
            return false;
        }
    }


    return true;
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

gboolean set_view_center_object(gpointer dataPointer)
{
    BattlegroundDynamic_element *object = showCharacterPointer;
    if ((object->objectData) == NULL)
        return TRUE;

    CharacterData *data = (CharacterData *)(object->objectData);
    gdouble newX = object->posX - windowWidth / 2;
    gdouble newY = object->posY - windowHeight / 2;
    gtk_adjustment_set_value(data->hadj, newX);
    gtk_adjustment_set_value(data->vadj, newY);

    return TRUE;
}




