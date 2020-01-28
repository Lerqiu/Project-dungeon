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
#include "Object_monster_events.h"
#include "Object_princess_events.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"
#include "Battleground_set_view.h"

extern int defaultCharTabLength;

extern BattlegroundDynamic *dynamic_objects_on_map;
extern BattlegroundDynamic_element *mainCharacter;
extern BattlegroundDynamic_element *showCharacterPointer;

static void characterChangeBodyView(BattlegroundDynamic_element *character);

//Synchronizacja danych

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
    characterChangeBodyView(object);

    free(poi);
}

void Synchronization_character_dead(BattlegroundDynamic_element *character)
{
    characterClearKeys(character);
    free(character->type);
    character->type = (char *)malloc(sizeof(char) * defaultCharTabLength);
    strcpy(character->type, "dead_character");

    gtk_image_clear(GTK_IMAGE(character->image));
    gtk_container_remove(GTK_CONTAINER(character->layout), character->image);
    character->image = NULL;

    if (character->indexStartPointX == mainCharacter->indexStartPointX &&
        character->indexStartPointY == mainCharacter->indexStartPointY)
    {
        showEndInfoAboutGame("Umarłeś", -1000 * 3);
    }
    BattlegroundDynamic *characters = getObjectByType("character");
    if (characters->amount == 0)
    {
        deactive_newSynchronizationEvent();
        deactive_Monsters();
        showEndInfoAboutGame("Remis", 1000 * 3);
        return;
    }

    for (int i = 0; i < characters->amount; i++)
    {
        showCharacterPointer = characters->tabOfElements[i];
    }
}

void character_clear_type(void)
{
    BattlegroundDynamic *characters = getObjectByType("character");

    for (int i = 0; i < characters->amount; i++)
    {
        free(characters->tabOfElements[i]->type);
        characters->tabOfElements[i]->type = (char *)malloc(sizeof(char) * defaultCharTabLength);
        strcpy(characters->tabOfElements[i]->type, "dead_character");
    }
}

void Synchronization_character_win(BattlegroundDynamic_element *character)
{
    if (character->indexStartPointX == mainCharacter->indexStartPointX &&
        character->indexStartPointY == mainCharacter->indexStartPointY)
    {
        showEndInfoAboutGame("Wygrałeś", 1000 * 3);
    }
    else
    {
        showEndInfoAboutGame("Przegrałeś", 1000 * 3);
    }
    character_clear_type();
    deactive_newSynchronizationEvent();
    deactive_Monsters();
}

//Zmiana wyglądu

static void characterChangeBodyView(BattlegroundDynamic_element *character)
{
    CharacterData *chData = (CharacterData *)character->objectData;

    int direction = chData->directionForImages;
    if (character->posX - chData->previousPosX > 0)
        direction = 3;
    if (character->posX - chData->previousPosX < 0)
        direction = 1;
    if (character->posY - chData->previousPosY > 0)
        direction = 2;
    if (character->posY - chData->previousPosY < 0)
        direction = 0;

    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 64, 64);

    gdk_pixbuf_scale(gtk_image_get_pixbuf(GTK_IMAGE(character->viewData)), pixbuf, 0, 0, 64, 64, -64 * chData->indexOfFrame, -64 * direction, 1.0, 1.0, GDK_INTERP_NEAREST);
    gtk_image_clear(GTK_IMAGE(character->image));

    gtk_image_set_from_pixbuf(GTK_IMAGE(character->image), pixbuf);

    g_object_unref(pixbuf);
    chData->indexOfFrame++;
    if (chData->indexOfFrame > 8)
        chData->indexOfFrame = 0;

    chData->directionForImages = direction;

    chData->previousPosX = character->posX;
    chData->previousPosY = character->posY;
}

//Pozostałe

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
            characterChangeBodyView(object);
        }
    }
    characterGetKey(object);

    characterStepOnTrap(object);
    princes_save(object);
}

void objects_movie_up(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, 0, -object->speed);
}
void objects_movie_right(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, object->speed, 0);

    //printf("right\n");
}
void objects_movie_down(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, 0, +object->speed);

    //printf("dowm\n");
}
void objects_movie_left(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;

    character_move_set_on_layout(object, -object->speed, 0);
    // printf("left\n");
}

gboolean character_movie_keyboard_reverse(GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)data;
    if (strcmp(object->type, "character"))
        return TRUE;

    if (event->keyval == GDK_KEY_w || event->keyval == GDK_KEY_W)
    {
        objects_movie_up(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_d || event->keyval == GDK_KEY_D)
    {
        objects_movie_right(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_s || event->keyval == GDK_KEY_S)
    {
        objects_movie_down(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_a || event->keyval == GDK_KEY_A)
    {
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
        objects_movie_up(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Right)
    {
        objects_movie_right(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Down)
    {
        objects_movie_down(data);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Left)
    {
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