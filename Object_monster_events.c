#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include <gtk/gtk.h>

#include "Object_monster_events.h"

#include "Object_character_events.h"
#include "Object_gate_events.h"
#include "Objects_events.h"
#include "Objects_events_synchronization.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

extern int DEF_IMAGE_SIZE;
extern int defaultCharTabLength;
extern int defaultMonsterAttackRangeSquares;

void Synchronization_move_monster(Pointer_and_Index *poi, char event[])
{
    if (strcmp("monster", poi->pointer->type))
        return;

    int X, Y;
    int direction, indexFrame;
    if (sscanf(event, "move-%i-%i-imageView-%i-%i", &Y, &X, &direction, &indexFrame) < 4)
        return;

    if (poi == NULL)
        return;

    BattlegroundDynamic_element *object = poi->pointer;
    MonsterData *mData = (MonsterData *)object->objectData;
    mData->direction = direction;
    mData->indexOfFrame = indexFrame;

    object->posX = X;
    object->posY = Y;

    if (object->image != NULL)
    {
        gtk_fixed_move(GTK_FIXED(object->layout), object->image, X, Y);

        monsterChangeBodyView(object);
    }
    free(poi);
}

bool checkIfAttacked(BattlegroundDynamic_element *monster, BattlegroundDynamic_element *character)
{
    if (monster->objectData == NULL)
        return false;
    MonsterData *mData = (MonsterData *)monster->objectData;

    int posXMonster = (monster->posX + monster->pivotPosX);
    int posYMonster = (monster->posY + monster->pivotPosY);

    int posXCharacter = (character->posX + character->pivotPosX) / DEF_IMAGE_SIZE;
    int posYCharacter = (character->posY + character->pivotPosY) / DEF_IMAGE_SIZE;

    int direction = mData->direction;

    if (posYCharacter == posYMonster / DEF_IMAGE_SIZE)
    {
        if (direction == 1)
            for (int in = 0; in < defaultMonsterAttackRangeSquares; in++)
            {
                if (isAPath(posXMonster + in * DEF_IMAGE_SIZE, posYMonster) && !isThereAGate(posYMonster / DEF_IMAGE_SIZE, (posXMonster + in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE))
                {
                    if (posXCharacter == (posXMonster + in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE && posYCharacter == posYMonster / DEF_IMAGE_SIZE)
                    {
                        return true;
                    }
                }
                else
                {
                    break;
                }
            }
        if (direction == 3)
            for (int in = 0; in < defaultMonsterAttackRangeSquares; in++)
            {
                if (isAPath(posXMonster - in * DEF_IMAGE_SIZE, posYMonster) && !isThereAGate(posYMonster / DEF_IMAGE_SIZE, (posXMonster - in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE))
                {
                    if (posXCharacter == (posXMonster - in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE && posYCharacter == posYMonster / DEF_IMAGE_SIZE)
                    {
                        return true;
                    }
                }
                else
                {
                    break;
                }
            }
    }

    if (posXCharacter == posXMonster / DEF_IMAGE_SIZE)
    {
        if (direction == 2)
            for (int in = 0; in < defaultMonsterAttackRangeSquares; in++)
            {
                if (isAPath(posXMonster, posYMonster + in * DEF_IMAGE_SIZE) && !isThereAGate((posYMonster + in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE, posXMonster / DEF_IMAGE_SIZE))
                {
                    if (posXCharacter == posXMonster / DEF_IMAGE_SIZE && posYCharacter == (posYMonster + in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE)
                    {
                        return true;
                    }
                }
                else
                {
                    break;
                }
            }
        if (direction == 0)
            for (int in = 0; in < defaultMonsterAttackRangeSquares; in++)
            {
                if (isAPath(posXMonster, posYMonster - in * DEF_IMAGE_SIZE) && !isThereAGate((posYMonster - in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE, posXMonster / DEF_IMAGE_SIZE))
                {
                    if (posXCharacter == posXMonster / DEF_IMAGE_SIZE && posYCharacter == (posYMonster - in * DEF_IMAGE_SIZE) / DEF_IMAGE_SIZE)
                    {
                        return true;
                    }
                }
                else
                {
                    break;
                }
            }
    }

    return false;
}

void monsterChangeBodyView(BattlegroundDynamic_element *monster)
{
    MonsterData *mData = (MonsterData *)monster->objectData;

    int direction = mData->direction;
    if (direction == 1)
        direction = 3;
    else if (direction == 3)
        direction = 1;

    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 64, 64);

    gdk_pixbuf_scale(gtk_image_get_pixbuf(GTK_IMAGE(monster->viewData)), pixbuf, 0, 0, 64, 64, -64, -64 * direction, 1.0, 1.0, GDK_INTERP_NEAREST);
    //g_object_unref(gtk_image_get_pixbuf(GTK_IMAGE(monster->image)));
    gtk_image_clear(GTK_IMAGE(monster->image));

    gtk_image_set_from_pixbuf(GTK_IMAGE(monster->image), pixbuf);

    g_object_unref(pixbuf);
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
                //printf("Check\n");
                //characterDead(characters->tabOfElements[i]);
                newSmallSynchronizationEvent(characters->tabOfElements[i], "dead");
            }
        }
}

void monster_move_set_on_layout(BattlegroundDynamic_element *object, int oX, int oY)
{
    if (object == NULL)
        return;
    if (strcmp(object->type, "monster"))
        return;

    int x = object->posX + oX + object->pivotPosX;
    int y = object->posY + oY + object->pivotPosY;

    if (isHumanoidOnPath(object, x, y) == true && humanoidColisionGate(object, x, y) == false)
    {
        object->posX = object->posX + oX;
        object->posY = object->posY + oY;

        MonsterData *mData = (MonsterData *)object->objectData;

        if (object->image != NULL)
        {
            char action[defaultCharTabLength];
            sprintf(action, "move-%i-%i-imageView-%i-%i", object->posY, object->posX, mData->direction, mData->indexOfFrame);
            newSmallSynchronizationEvent(object, action);
            gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
        }
    }
    else
    {
        MonsterData *mData = (MonsterData *)object->objectData;
        mData->isColision = true;
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

        monster_move_set_on_layout(monsters->tabOfElements[i], oX, oY);
    }
    monstersMakeAttac();
    return TRUE;
}