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

extern GtkAdjustment *hadj;
extern GtkAdjustment *vadj;

extern int windowHeight;
extern int windowWidth;

extern BattlegroundStatic *static_objects_on_map;
extern int DEF_IMAGE_SIZE;

bool isColisionCircle_Square(BattlegroundDynamic_element *object, int X, int Y)
{
    if (strcmp("path", static_objects_on_map->map[(Y / DEF_IMAGE_SIZE) * static_objects_on_map->X + X / DEF_IMAGE_SIZE]->type))
        return true;

    printf("%i %i\n", (Y / DEF_IMAGE_SIZE) * static_objects_on_map->X, X / DEF_IMAGE_SIZE);
    for (int i = 0; i < 8; i++)
    {
        if (strcmp("path", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y)) / DEF_IMAGE_SIZE) * static_objects_on_map->X + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX) + X) / DEF_IMAGE_SIZE)]->type))
        {
            printf("%s %i %i\n", static_objects_on_map->map[(int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y) / DEF_IMAGE_SIZE) * static_objects_on_map->X) + (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX + X) / DEF_IMAGE_SIZE))]->type, (int)(((sin((2.0 / 8.0 * i * 3.14)) * object->colisionCheckY + Y) / DEF_IMAGE_SIZE) * static_objects_on_map->X), (int)(((cos(2.0 / 8.0 * i * 3.14) * object->colisionCheckX + X) / DEF_IMAGE_SIZE)));
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

static void make_move(BattlegroundDynamic_element *object, int oX, int oY)
{
    int x = object->posX + oX + object->pivotPosX;
    int y = object->posY + oY + object->pivotPosY;

    // printf("y:%i x:%i\n", y, x);

    if (isColisionCircle_Square(object, x, y) == false)
    {
        object->posX = object->posX + oX;
        object->posY = object->posY + oY;
        if (object->image != NULL)
            gtk_layout_move(GTK_LAYOUT(object->layout), object->image, object->posX, object->posY);
    }
}

void set_view_center_By_Character(void *ob)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)ob;
    //printf("Make view movie\n");
    if ((object->objectData) == NULL)
        return;

    //printf(" PosX:%i PosY:%i\n",object->posX,object->posY);

    CharacterData *data = (CharacterData *)(object->objectData);

    gtk_adjustment_set_value(hadj, object->posX - windowWidth / 2);
    gtk_adjustment_set_value(vadj, object->posY - windowHeight / 2);
}

void objects_movie_up(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;
    if (!strcmp(object->type, "character"))
    {
        make_move(object, 0, -defaultCharacterSpeed);
        set_view_center_By_Character(object);
    }
    else
    {
        make_move(object, 0, -defaultMonsterSpeed);
    }

    //printf("UP\n");
}
void objects_movie_right(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;
    if (!strcmp(object->type, "character"))
    {
        make_move(object, defaultCharacterSpeed, 0);
        set_view_center_By_Character(object);
    }
    else
    {
        make_move(object, defaultMonsterSpeed, 0);
    }
    //printf("right\n");
}
void objects_movie_down(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;
    if (!strcmp(object->type, "character"))
    {
        make_move(object, 0, +defaultCharacterSpeed);
        set_view_center_By_Character(object);
    }
    else
    {
        make_move(object, 0, +defaultMonsterSpeed);
    }
    //printf("dowm\n");
}
void objects_movie_left(gpointer *pointer)
{
    BattlegroundDynamic_element *object = (BattlegroundDynamic_element *)pointer;
    if (!strcmp(object->type, "character"))
    {
        make_move(object, -defaultCharacterSpeed, 0);
        set_view_center_By_Character(object);
    }
    else
    {
        make_move(object, -defaultMonsterSpeed, 0);
    }
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

    return FALSE;
}