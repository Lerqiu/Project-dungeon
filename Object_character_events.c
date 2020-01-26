#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>

#include "Object_character_events.h"
#include "Objects_events.h"
#include "Objects_events_synchronization.h"
#include "Object_key_events.h"
#include "Object_gate_events.h"
#include "Object_trap_events.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

extern int windowHeight;
extern int windowWidth;
extern int defaultCharTabLength;

extern BattlegroundDynamic *dynamic_objects_on_map;

void Synchronization_move_character(Pointer_and_Index *poi, char event[])
{
    if (strcmp("character", poi->pointer->type))
        return;

    int X, Y;
    if (sscanf(event, "move-%i-%i", &Y, &X) < 2)
        return;

    if (poi == NULL)
        return;

    BattlegroundDynamic_element *object = poi->pointer;

    object->posX = X;
    object->posY = Y;

    if (object->image != NULL)
    {
        gtk_fixed_move(GTK_FIXED(object->layout), object->image, X, Y);
    }
    free(poi);
}

void character_move_set_on_layout(BattlegroundDynamic_element *object, int oX, int oY)
{
    if (object == NULL)
        return;

    if (strcmp(object->type, "character"))
        return;

    int x = object->posX + oX + object->pivotPosX;
    int y = object->posY + oY + object->pivotPosY;

    if (isHumanoidOnPath(object, x, y) == true && humanoidColisionGate(object, x, y) == false)
    {
        object->posX = object->posX + oX;
        object->posY = object->posY + oY;
        if (object->image != NULL)
        {
            char action[defaultCharTabLength];
            sprintf(action, "move-%i-%i", object->posY, object->posX);
            newSmallSynchronizationEvent(object, action);
            gtk_fixed_move(GTK_FIXED(object->layout), object->image, object->posX, object->posY);
        }
    }
    characterGetKey(object);

    characterStepOnTrap(object);
    characterSavePrinces(object);
}

void Synchronization_character_dead(BattlegroundDynamic_element *character)
{
    printf("You are dead, indexX:%i indexY:%i!!\n", character->indexStartPointX, character->indexStartPointY);
    characterClearKeys(character);
    free(character->type);
    character->type = (char *)malloc(sizeof(char) * defaultCharTabLength);
    strcpy(character->type, "dead_character");

    gtk_image_clear(GTK_IMAGE(character->image));
    gtk_container_remove(GTK_CONTAINER(character->layout), character->image);
    character->image = NULL;
}
void Synchronization_character_win(BattlegroundDynamic_element *character)
{
    printf("Princess is saved!!\n");
}

bool characterSavePrinces(BattlegroundDynamic_element *object)
{
    if (object == NULL)
        return false;
    if (strcmp(object->type, "character"))
        return false;

    BattlegroundDynamic *princess = getObjectByType("princess");

    if (princess->amount < 1)
        return false;

    if (isCharacterInRangeOfAction(object, princess->tabOfElements[0]))
    {
        //Uratowanie księżniczki
        if (!strcmp(object->type, "character"))
        {
            
            newSmallSynchronizationEvent(object, "princessSaved");
        }

        return true;
    }

    return false;
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

    character_move_set_on_layout(object, 0, -object->speed);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);
}
void objects_movie_right(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, object->speed, 0);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);

    //printf("right\n");
}
void objects_movie_down(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, 0, +object->speed);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);

    //printf("dowm\n");
}
void objects_movie_left(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, -object->speed, 0);
    if (!strcmp(object->type, "character"))
        set_view_center_By_Character(object);
    // printf("left\n");
}

gboolean character_movie_keyboard_reverse(GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)data;
    if (strcmp(object->type, "character"))
        return TRUE;

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

gboolean character_movie_keyboard(GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)data;
    if (strcmp(object->type, "character"))
        return TRUE;

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