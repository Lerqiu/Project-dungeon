#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "Object_character_events.h"
#include "Objects_events.h"
#include "Objects_events_synchronization.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

extern int DEF_IMAGE_SIZE;

bool checkIfAttacked(BattlegroundDynamic_element *monster, BattlegroundDynamic_element *character)
{
    if (monster->objectData == NULL)
        return false;
    MonsterData *mData = (MonsterData *)monster->objectData;

    int posXMonster = (monster->posX + monster->pivotPosX) / DEF_IMAGE_SIZE;
    int posYMonster = (monster->posY + monster->pivotPosY) / DEF_IMAGE_SIZE;

    int posXCharacter = (character->posX + character->pivotPosX) / DEF_IMAGE_SIZE;
    int posYCharacter = (character->posY + character->pivotPosY) / DEF_IMAGE_SIZE;

    if (posYCharacter == posYMonster)
    {
        if (mData->direction == 1)
        {
            if (posXCharacter - posXMonster <= 3 && posXCharacter - posXMonster >= 0)
                return true;
        }
        if (mData->direction == 3)
        {
            if (posXCharacter - posXMonster >= -3 && posXCharacter - posXMonster <= 0)
                return true;
        }
    }

    if (posXCharacter == posXMonster)
    {
        if (mData->direction == 0)
        {
            if (posYCharacter - posXMonster <= 0 && posYCharacter - posXMonster >= -3)
                return true;
        }
        if (mData->direction == 2)
        {
            if (posYCharacter - posXMonster <= 3 && posYCharacter - posXMonster >= 0)
                return true;
        }
    }

    return false;
}

void monstersMakeAttac(void)
{
    BattlegroundDynamic *characters = getObjectByType("character");

    if (characters->amount < 1)
        return;

    BattlegroundDynamic *monsters = getObjectByType("monster");

    for (int i = 0; i < characters->amount; i++)
        for (int y = 0; y < monsters->amount; y++)
        {
            if (checkIfAttacked(monsters->tabOfElements[y], characters->tabOfElements[i]))
            {
                characterDead(characters->tabOfElements[i]);
            }
        }
}

gboolean monster_move(gpointer data)
{

    BattlegroundDynamic *monsters = getObjectByType("monster");

    for (int i = 0; i < monsters->amount; i++)
    {
       
                if (monsters->tabOfElements[i]->objectData == NULL)
                    continue;

                MonsterData *mData = (MonsterData *)monsters->tabOfElements[i]->objectData;
                int oX = 0, oY = 0;
                int defaultMonsterSpeed = monsters->tabOfElements[i]->speed;

                if (mData->isColision == true)
                {
                    if (mData->direction == 0)
                        mData->direction = 2;
                    else if (mData->direction == 1)
                        mData->direction = 3;
                    else if (mData->direction == 2)
                        mData->direction = 0;
                    else if (mData->direction == 3)
                        mData->direction = 1;
                    mData->isColision = false;
                }

                if (mData->direction == 1)
                    oX += defaultMonsterSpeed;
                else if (mData->direction == 3)
                    oX -= defaultMonsterSpeed;

                if (mData->direction == 2)
                    oY += defaultMonsterSpeed;
                else if (mData->direction == 0)
                    oY -= defaultMonsterSpeed;

                make_move(monsters->tabOfElements[i], oX, oY);
    }
    monstersMakeAttac();
    return TRUE;
}