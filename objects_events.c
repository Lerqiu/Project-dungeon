#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <math.h>

#include "objects_events.h"
#include "settings.h"
#include "battleground.h"

extern int defaultMonsterSpeed;
extern int defaultCharacterSpeed;

extern int windowHeight;
extern int windowWidth;

extern BattlegroundStatic *static_objects_on_map;
extern BattlegroundDynamic *dynamic_objects_on_map;
extern int DEF_IMAGE_SIZE;
extern int maxLengthOfPath;

bool isCharacterOnPath(BattlegroundDynamic_element *object, int X, int Y)
{
    if (strcmp("path", static_objects_on_map->map[(Y / DEF_IMAGE_SIZE) * static_objects_on_map->X + X / DEF_IMAGE_SIZE]->type))
        return true;

    //printf("%i %i\n", (Y / DEF_IMAGE_SIZE) * static_objects_on_map->X, X / DEF_IMAGE_SIZE);
    for (int i = 0; i < 8; i++)
    {
        if (strcmp("path", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y)) / DEF_IMAGE_SIZE) * static_objects_on_map->X + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX) + X) / DEF_IMAGE_SIZE)]->type))
        {
            //printf("%s %i %i\n", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y) / DEF_IMAGE_SIZE) * static_objects_on_map->X) + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX + X) / DEF_IMAGE_SIZE))]->type, (int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y) / DEF_IMAGE_SIZE) * static_objects_on_map->X), (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX + X) / DEF_IMAGE_SIZE)));
            return true;
        }
    }

    /*int y = Y - 3;
    if (y < 0)
        y = 0;
    int x = X - 3;
    if (x < 0)
        x = 0;
        
    for (; y < static_objects_on_map->Y; y++)
    {
        for (; x < static_objects_on_map->X; x++)
        {
            if ((static_objects_on_map->map[y * static_objects_on_map->X + x]->posX + static_objects_on_map->map[y * static_objects_on_map->X + x]->pivotPosX + static_objects_on_map->map[y * static_objects_on_map->X + x]->colisionCheckX) >= (object->posX + object->pivotPosX) 
            &&(static_objects_on_map->map[y * static_objects_on_map->X + x]->posX + static_objects_on_map->map[y * static_objects_on_map->X + x]->pivotPosX - static_objects_on_map->map[y * static_objects_on_map->X + x]->colisionCheckX) <= (object->posX + object->pivotPosX)
            &&(static_objects_on_map->map[y * static_objects_on_map->X + x]->posY + static_objects_on_map->map[y * static_objects_on_map->X + x]->pivotPosY + static_objects_on_map->map[y * static_objects_on_map->X + x]->colisionCheckY) <= (object->posY + object->pivotPosY)
            &&(static_objects_on_map->map[y * static_objects_on_map->X + x]->posY + static_objects_on_map->map[y * static_objects_on_map->X + x]->pivotPosY - static_objects_on_map->map[y * static_objects_on_map->X + x]->colisionCheckY) >= (object->posY + object->pivotPosY))
                return true;
        }
    }*/

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
                //printf("Character posY:%i posX:%i pivY:%i pivX:%i suma Y:%i X:%i\n",character_or_monster->posY, character_or_monster->posX ,character_or_monster->pivotPosY, character_or_monster->pivotPosX,character_or_monster->posY + character_or_monster->pivotPosY,character_or_monster->posX + character_or_monster->pivotPosX);
                //printf("Gate posY:%i posX:%i pivY:%i pivX:%i suma Y:%i X:%i s:%i\n",obj->posY, obj->posX ,obj->pivotPosY, obj->pivotPosX,obj->posY + obj->pivotPosY,obj->posX + obj->pivotPosX,obj->indexStartPointY);
                //printf("y:%i x:%i\n",y,x);

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

bool characterColisionGate(BattlegroundDynamic_element *object, int oX, int oY)
{
    int x, y;
    x = object->posX;
    y = object->posY;
    object->posX = oX - object->pivotPosX;
    object->posY = oY - object->pivotPosY;

    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "gate"))
            {
                GateData *g = (GateData *)dynamic_objects_on_map->tabOfElements[i]->objectData;

                if (g->isOpen == false && isColisionDynamic(object, dynamic_objects_on_map->tabOfElements[i]))
                {
                    object->posX = x;
                    object->posY = y;
                    return true;
                }
            }
    }
    object->posX = x;
    object->posY = y;

    return false;
}

void characterGetKey(BattlegroundDynamic_element *object)
{
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "key"))
            {
                if (isColisionDynamic(object, dynamic_objects_on_map->tabOfElements[i]))
                {
                    KeyData *k = (KeyData *)dynamic_objects_on_map->tabOfElements[i]->objectData;
                    CharacterData *ch = (CharacterData *)object->objectData;

                    if (k->key_type < 4 && k->key_type >= 0)
                    {
                        ch->keyTab[k->key_type]++;

                        extern GtkWidget *labelsTabRightPanel[4];
                        char buffer[100];
                        sprintf(buffer, "%i", ch->keyTab[k->key_type]);
                        gtk_label_set_text(GTK_LABEL(labelsTabRightPanel[k->key_type]), buffer);

                        newSmallSynchronizationEvent(dynamic_objects_on_map->tabOfElements[i], "vanish");
                    }

                    /*
                    gtk_image_clear(GTK_IMAGE(dynamic_objects_on_map->tabOfElements[i]->image));
                    gtk_container_remove(GTK_CONTAINER(dynamic_objects_on_map->tabOfElements[i]->layout), dynamic_objects_on_map->tabOfElements[i]->image);
                    dynamic_objects_on_map->tabOfElements[i]->image = NULL;

                    free(dynamic_objects_on_map->tabOfElements[i]);
                    dynamic_objects_on_map->tabOfElements[i] = NULL;
                    */
                }
            }
    }
}

void characterKeyUse(BattlegroundDynamic_element *object, int key)
{
    CharacterData *ch = (CharacterData *)object->objectData;
    if (key >= 4 || key < 0)
        return;
    if (ch->keyTab[key] <= 0)
        return;

    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "gate"))
            {

                if (isCharacterInRangeOfAction(object, dynamic_objects_on_map->tabOfElements[i]))
                {
                    if(dynamic_objects_on_map->tabOfElements[i]==NULL)
                        continue;
                    GateData *g = (GateData *)dynamic_objects_on_map->tabOfElements[i]->objectData;
                    if (g->key_type == key && !g->isOpen)
                    {
                        ch->keyTab[key]--;
                        extern GtkWidget *labelsTabRightPanel[4];
                        char buffer[100];
                        sprintf(buffer, "%i", ch->keyTab[key]);
                        gtk_label_set_text(GTK_LABEL(labelsTabRightPanel[key]), buffer);

                        newSmallSynchronizationEvent(dynamic_objects_on_map->tabOfElements[i], "open");
                        return;
                        /*
                       
                        g->isOpen = true;

                        gtk_image_clear(GTK_IMAGE(dynamic_objects_on_map->tabOfElements[i]->image));
                        gtk_container_remove(GTK_CONTAINER(dynamic_objects_on_map->tabOfElements[i]->layout), dynamic_objects_on_map->tabOfElements[i]->image);
                        dynamic_objects_on_map->tabOfElements[i]->image = NULL;

                        free(dynamic_objects_on_map->tabOfElements[i]);
                        dynamic_objects_on_map->tabOfElements[i] = NULL;
                        */
                    }
                }
            }
    }
}

bool characterStepOnTrap(BattlegroundDynamic_element *object)
{
    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "trap"))
            {
                if (isColisionDynamic(object, dynamic_objects_on_map->tabOfElements[i]))
                {
                    //Wejście na pułapkę
                    if (!strcmp(object->type, "character"))
                    {
                        printf("You died!! funkcja:characterStepOnTrap\n");
                        newSmallSynchronizationEvent(object, "dead");
                        newSmallSynchronizationEvent(dynamic_objects_on_map->tabOfElements[i], "launched");
                    }

                    return true;
                }
            }
    }
    return false;
}

bool characterSavePrinces(BattlegroundDynamic_element *object)
{
    if (object == NULL)
        return false;
    if (strcmp(object->type, "character"))
        return false;

    for (int i = 0; i < dynamic_objects_on_map->amount; i++)
    {
        if (dynamic_objects_on_map->tabOfElements[i] != NULL)
            if (!strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "princess"))
            {
                if (isCharacterInRangeOfAction(object, dynamic_objects_on_map->tabOfElements[i]))
                {
                    //Uratowanie księżniczki
                    if (!strcmp(object->type, "character"))
                    {
                        printf("Princess is saved!! funkcja:characterSavePrinces\n");
                        newSmallSynchronizationEvent(object, "princessSaved");
                    }

                    return true;
                }
            }
    }
    return false;
}

static void make_move(BattlegroundDynamic_element *object, int oX, int oY)
{
    if (object == NULL)
        return;

    int x = object->posX + oX + object->pivotPosX;
    int y = object->posY + oY + object->pivotPosY;

    if (!strcmp(object->type, "character"))
    {
        if (isCharacterOnPath(object, x, y) == false && characterColisionGate(object, x, y) == false)
        {
            object->posX = object->posX + oX;
            object->posY = object->posY + oY;
            if (object->image != NULL)
            {
                //gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
                char action[maxLengthOfPath];
                sprintf(action, "move-%i-%i", object->posY, object->posX);
                newSmallSynchronizationEvent(object, action);
                gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
            }
        }
        characterGetKey(object);
        // for (int i = 0; i < 4; i++)
        // characterKeyUse(object, i);

        characterStepOnTrap(object);
        characterSavePrinces(object);
    }
    else
    {
        if (strcmp(object->type, "monster"))
            return;
        if (isCharacterOnPath(object, x, y) == false)
        {
            bool colision = false;

            for (int i = 0; i < dynamic_objects_on_map->amount; i++)
            {
                if (dynamic_objects_on_map->tabOfElements[i] != NULL)
                    if (strcmp(dynamic_objects_on_map->tabOfElements[i]->type, "character"))
                    {
                        if (isColisionDynamic(object, dynamic_objects_on_map->tabOfElements[i]))
                        {
                            colision = true;
                            break;
                        }
                    }
            }

            if (colision == false)
            {
                object->posX = object->posX + oX;
                object->posY = object->posY + oY;
                if (object->image != NULL)
                {
                    newSmallSynchronizationEvent(object, "move");
                }
            }
            else
            {

                MonsterData *m = (MonsterData *)object->objectData;
                m->direction -= 2;
                if (m->direction < 0)
                    m->direction = 4 + m->direction;
            }
        }
    }
}

void set_view_center_By_Character(void *ob)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)ob;
    if ((object->objectData) == NULL)
        return;

    CharacterData *data = (CharacterData *)(object->objectData);
    gdouble newX = object->posX - windowWidth / 2;
    gdouble newY = object->posY - windowHeight / 2;
    gtk_adjustment_set_value(data->hadj, newX);
    gtk_adjustment_set_value(data->vadj, newY);
}

void objects_movie_up(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    make_move(object, 0, -object->speed);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);
}
void objects_movie_right(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    make_move(object, object->speed, 0);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);

    //printf("right\n");
}
void objects_movie_down(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    make_move(object, 0, +object->speed);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);

    //printf("dowm\n");
}
void objects_movie_left(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    make_move(object, -object->speed, 0);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);
    // printf("left\n");
}

gboolean objects_movie_Key_Reverse(GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    if (event->keyval == GDK_KEY_w || event->keyval == GDK_KEY_W)
    {
        // printf(u8"Key pressed, go up objects_event.c keyBoard type:Reversed\n");
        objects_movie_up(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_d || event->keyval == GDK_KEY_D)
    {
        //printf(u8"Key pressed, right up objects_event.c keyBoard type:Reversed\n");
        objects_movie_right(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_s || event->keyval == GDK_KEY_S)
    {
        //printf(u8"Key pressed, go down objects_event.c keyBoard type:Reversed\n");
        objects_movie_down(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_a || event->keyval == GDK_KEY_A)
    {
        //printf(u8"Key pressed, go left objects_event.c keyBoard type:Reversed\n");
        objects_movie_left(data);
        return TRUE;
    }

    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)data;
    if (event->keyval == GDK_KEY_1)
    {
        characterKeyUse(object, 0);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_2)
    {
        characterKeyUse(object, 1);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_3)
    {
        characterKeyUse(object, 2);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_4)
    {
        characterKeyUse(object, 3);

        return TRUE;
    }

    return FALSE;
}

gboolean objects_movie(GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    if (event->keyval == GDK_KEY_Up)
    {
        //printf(u8"Key pressed, go up objects_event.c keyBoard type:Normal\n");
        objects_movie_up(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Right)
    {
        //printf(u8"Key pressed, go right objects_event.c keyBoard type:Normal\n");
        objects_movie_right(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Down)
    {
        //printf(u8"Key pressed, go down objects_event.c keyBoard type:Normal\n");
        objects_movie_down(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Left)
    {
        //printf(u8"Key pressed, go left objects_event.c keyBoard type:Normal\n");
        objects_movie_left(data);
        return TRUE;
    }

    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)data;
    if (event->keyval == GDK_KEY_V || event->keyval == GDK_KEY_v)
    {
        characterKeyUse(object, 0);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_B || event->keyval == GDK_KEY_b)
    {
        characterKeyUse(object, 1);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_N || event->keyval == GDK_KEY_n)
    {
        characterKeyUse(object, 2);

        return TRUE;
    }
    else if (event->keyval == GDK_KEY_M || event->keyval == GDK_KEY_m)
    {
        characterKeyUse(object, 3);

        return TRUE;
    }

    return FALSE;
}