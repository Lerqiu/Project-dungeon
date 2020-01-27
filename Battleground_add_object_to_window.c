#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

#include "Menu_window_functionalities.h"
#include "Default_settings.h"

#include "Map_loader_prototype.h"
#include "Map_loader_statics_elements.h"
#include "Map_loader_dynamic_elements.h"

#include "Objects_basic_types.h"

extern BattlegroundStatic *static_objects_on_map;
extern BattlegroundDynamic *dynamic_objects_on_map;

extern bool isServer;
extern int MainCharacterServerSearchMapIndex;

extern BattlegroundDynamic_element *mainCharacter;
extern BattlegroundDynamic_element *showCharacterPointer;

void create_battleground_static(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay)
{
    static_objects_on_map = (BattlegroundStatic *)malloc(sizeof(BattlegroundStatic));
    static_objects_on_map->X = pr_map->X;
    static_objects_on_map->Y = pr_map->Y;
    static_objects_on_map->map = (BattlegroundStatic_element **)malloc(sizeof(BattlegroundStatic_element *) * pr_map->Y * pr_map->X);

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            static_objects_on_map->map[i * static_objects_on_map->X + q] = load_battleground_static((pr_map->map + i * pr_map->X + q), q, i, pr_map);
        }
    }

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if ((static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage != NULL)
                gtk_fixed_put(GTK_FIXED(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->downimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }
}

void create_battleground__static_top(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay)
{

    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if ((static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage != NULL)
                gtk_fixed_put(GTK_FIXED(lay), (static_objects_on_map->map)[i * static_objects_on_map->X + q]->topimage, static_objects_on_map->map[i * static_objects_on_map->X + q]->posX, static_objects_on_map->map[i * static_objects_on_map->X + q]->posY);
        }
    }
}

void create_character(BattlegroundDynamic *map, GtkAdjustment *hadjCharacter, GtkAdjustment *vadjCharacter)
{
    BattlegroundDynamic_element *character[2];

    int b = 0;
    for (int a = 0; a < map->amount; a++)
    {
        if (!strcmp(map->tabOfElements[a]->type, "character"))
        {
            character[b] = map->tabOfElements[a];
            CharacterData *characData = (CharacterData *)(character[b]->objectData);
            characData->hadj = hadjCharacter;
            characData->vadj = vadjCharacter;
            b++;
        }
    }
    extern int characterServerIndexX;
    extern int characterServerIndexY;
    extern char *characterImagePathServer;

    extern int characterHostIndexX;
    extern int characterHostIndexY;
    extern char *characterImagePathHost;

    extern int MainCharacterServerSearchMapIndex;

    character[MainCharacterServerSearchMapIndex]->image = gtk_image_new_from_file(characterImagePathServer);
    character[1 - MainCharacterServerSearchMapIndex]->image = gtk_image_new_from_file(characterImagePathHost);

    if (isServer)
    {
        mainCharacter = character[MainCharacterServerSearchMapIndex];
        characterServerIndexX = character[MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterServerIndexY = character[MainCharacterServerSearchMapIndex]->indexStartPointY;

        characterHostIndexX = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterHostIndexY = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointY;
    }
    else
    {
        mainCharacter = character[1 - MainCharacterServerSearchMapIndex];
        characterServerIndexX = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterServerIndexY = character[1 - MainCharacterServerSearchMapIndex]->indexStartPointY;

        characterHostIndexX = character[MainCharacterServerSearchMapIndex]->indexStartPointX;
        characterHostIndexY = character[MainCharacterServerSearchMapIndex]->indexStartPointY;
    }
    showCharacterPointer = mainCharacter;
}

void battleground_draw_battleground_dynamic(BattlegroundDynamic *elements)
{
    for (int a = 0; a < elements->amount; a++)
    {
        if (elements->tabOfElements[a]->image != NULL)
        {
            gtk_fixed_put(GTK_FIXED(elements->tabOfElements[a]->layout), elements->tabOfElements[a]->image, elements->tabOfElements[a]->posX, elements->tabOfElements[a]->posY);
        }
    }
}

void create_battleground_dynamic(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay, GtkAdjustment *hadjCharacter, GtkAdjustment *vadjCharacter)
{
    dynamic_objects_on_map = (BattlegroundDynamic *)malloc(sizeof(BattlegroundDynamic));
    dynamic_objects_on_map->amount = amount_of_dynamic_elements(pr_map);
    dynamic_objects_on_map->tabOfElements = (BattlegroundDynamic_element **)malloc(sizeof(BattlegroundDynamic_element *) * (1 + dynamic_objects_on_map->amount));

    int a = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            dynamic_objects_on_map->tabOfElements[a] = NULL;
            dynamic_objects_on_map->tabOfElements[a] = load_battleground_dynamic((pr_map->map + i * pr_map->X + q), q, i);
            if (dynamic_objects_on_map->tabOfElements[a] != NULL)
            {
                dynamic_objects_on_map->tabOfElements[a]->layout = lay;
                a++;
            }
        }
    }

    create_character(dynamic_objects_on_map, hadjCharacter, vadjCharacter);

    //Rysowanie na mapie
    battleground_draw_battleground_dynamic(getObjectByType("trap"));
    battleground_draw_battleground_dynamic(getObjectByType("key"));
    battleground_draw_battleground_dynamic(getObjectByType("gate"));
    battleground_draw_battleground_dynamic(getObjectByType("monster"));
    battleground_draw_battleground_dynamic(getObjectByType("princess"));
    battleground_draw_battleground_dynamic(getObjectByType("character"));
}
