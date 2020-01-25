#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>

#include "Object_character_events.h"
#include "Objects_events.h"
#include "Objects_events_synchronization.h"
#include "Object_key_events.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

extern int windowHeight;
extern int windowWidth;

extern BattlegroundDynamic *dynamic_objects_on_map;

void characterDead(BattlegroundDynamic_element *character)
{
    printf("You are dead!!\n");
}
void characterWin(void)
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

    if(princess->amount<1)
        return false;

    if (isCharacterInRangeOfAction(object, princess->tabOfElements[0]))
    {
        //Uratowanie księżniczki
        if (!strcmp(object->type, "character"))
        {
            characterWin();
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

gboolean character_movie_keyboard_reverse(GtkWidget *widget, GdkEventKey *event, gpointer *data)
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

gboolean character_movie_keyboard(GtkWidget *widget, GdkEventKey *event, gpointer *data)
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