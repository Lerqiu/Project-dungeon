#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Map_loader_prototype.h"
#include "Map_loader_dynamic_elements.h"
#include "Default_settings.h"

extern int DEF_IMAGE_SIZE;
extern char folderPathDynamic[];
extern int defaultCharTabLength;
extern int defaultMonsterSpeed;
extern int defaultCharacterSpeed;
extern int defaultCharacterActionRangePx;

int amount_of_dynamic_elements(Prototype_map *pr_map)
{
    if (pr_map == NULL)
        return 0;

    int b = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {

            if (strcmp((pr_map->map + i * pr_map->X + q)->type_of_object, "w") && strcmp((pr_map->map + i * pr_map->X + q)->type_of_object, "p") && strcmp((pr_map->map + i * pr_map->X + q)->type_of_object, "n"))
                b++;
        }
    }
    return b;
}

void map_create_std_dynamic(int x, int y, BattlegroundDynamic_element *element, char type[])
{
    element->image = NULL;
    element->viewData = NULL;
    element->height = DEF_IMAGE_SIZE;
    element->width = DEF_IMAGE_SIZE;
    element->posX = x * DEF_IMAGE_SIZE;
    element->posY = y * DEF_IMAGE_SIZE;
    element->colisionCheckX = DEF_IMAGE_SIZE / 2;
    element->colisionCheckY = DEF_IMAGE_SIZE / 2;
    element->pivotPosX = DEF_IMAGE_SIZE / 2;
    element->pivotPosY = DEF_IMAGE_SIZE / 2;
    element->type = (char *)malloc(sizeof(char) * (strlen(type) + 1));
    strcpy(element->type, type);
    element->indexStartPointX = x;
    element->indexStartPointY = y;

    element->layout = NULL;

    element->actionRange = 0;
    element->speed = 0;

    element->objectData = NULL;
    element->viewData = NULL;
}

BattlegroundDynamic_element *load_battleground_dynamic(Prototype_map_element *prototype_element, int x, int y) //x i y to numery indeksu
{
    BattlegroundDynamic_element *element = (BattlegroundDynamic_element *)malloc(sizeof(BattlegroundDynamic_element));

    if (!strcmp(prototype_element->type_of_object, "e"))
    {
        map_create_std_dynamic(x, y, element, "princess");
        char pathToFile[defaultCharTabLength];
        sprintf(pathToFile, "%s%s%s", folderPathDynamic, "princess", ".png");
        element->image = gtk_image_new_from_file(pathToFile);
    }
    else if (!strcmp(prototype_element->type_of_object, "ch"))
    {
        map_create_std_dynamic(x, y, element, "character");
        element->speed = defaultCharacterSpeed;
        element->colisionCheckY = DEF_IMAGE_SIZE / 4;
        element->colisionCheckX = DEF_IMAGE_SIZE / 4;
        element->pivotPosY = element->height - DEF_IMAGE_SIZE / 4;
        element->actionRange = defaultCharacterActionRangePx;

        CharacterData *data = (CharacterData *)malloc(sizeof(CharacterData));
        data->hadj = NULL;
        data->vadj = NULL;
        data->indexOfFrame = 0;
        data->previousPosX = element->posX;
        data->previousPosY = element->posY;
        data->directionForImages=0;
        for (int i = 0; i < 4; i++)
            data->keyTab[i] = 0;
        element->objectData = (void *)data;
    }
    else if (!strcmp(prototype_element->type_of_object, "g"))
    {
        map_create_std_dynamic(x, y, element, "gate");
        GateData *data = (GateData *)malloc(sizeof(GateData));
        data->isOpen = false;
        data->key_type = prototype_element->v1;
        if (data->key_type < 0 || data->key_type > 3)
        {
            printf(u8"Error: Niepodano typu klucza otwierającego bramę Y:%i X:%i, błędna mapa!!!\n", y, x);
            exit(1);
        }
        char pathToFile[defaultCharTabLength];
        sprintf(pathToFile, "%s%s%i%s", folderPathDynamic, "gate_", data->key_type, ".png");
        element->image = gtk_image_new_from_file(pathToFile);

        element->objectData = (void *)data;
    }
    else if (!strcmp(prototype_element->type_of_object, "t"))
    {
        map_create_std_dynamic(x, y, element, "trap");
        char pathToFile[defaultCharTabLength];
        sprintf(pathToFile, "%s%s%s", folderPathDynamic, "trap", ".png");
        element->image = gtk_image_new_from_file(pathToFile);
    }
    else if (!strcmp(prototype_element->type_of_object, "m"))
    {
        map_create_std_dynamic(x, y, element, "monster");
        element->speed = defaultMonsterSpeed;
        element->colisionCheckY = DEF_IMAGE_SIZE / 4;
        element->colisionCheckX = DEF_IMAGE_SIZE / 4;
        element->pivotPosY = element->height - DEF_IMAGE_SIZE / 4;

        MonsterData *m = (MonsterData *)malloc(sizeof(MonsterData));
        m->direction = prototype_element->v1;
        m->amountsOfSteps = prototype_element->v2;
        m->endIndexX = element->indexStartPointX;
        m->endIndexY = element->indexStartPointY;
        m->isColision = false;
        m->indexOfFrame = 0;
        if (m->amountsOfSteps > 0)
        {
            if (m->direction == 0)
                m->endIndexY -= m->amountsOfSteps;
            if (m->direction == 1)
                m->endIndexX += m->amountsOfSteps;
            if (m->direction == 2)
                m->endIndexY += m->amountsOfSteps;
            if (m->direction == 3)
                m->endIndexX -= m->amountsOfSteps;
        }

        char pathToFile[defaultCharTabLength];
        //sprintf(pathToFile, "%s%s%s", folderPathDynamic, "monster", ".png");
        sprintf(pathToFile, "%s%s%s", folderPathDynamic, "skeleton", ".png");

        //element->image=NULL;
        element->viewData = gtk_image_new_from_file(pathToFile);
        GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 64, 64);
        gdk_pixbuf_scale(gtk_image_get_pixbuf(GTK_IMAGE(element->viewData)), pixbuf, 0, 0, 64, 64, 0, 0, 1.0, 1.0, GDK_INTERP_NEAREST);
        //element->image = gtk_image_new_from_file(pathToFile);
        element->image = gtk_image_new_from_pixbuf(pixbuf);

        g_object_unref(pixbuf);

        element->objectData = (void *)m;
    }
    else if (!strcmp(prototype_element->type_of_object, "k"))
    {
        map_create_std_dynamic(x, y, element, "key");
        KeyData *k = (KeyData *)malloc(sizeof(KeyData));
        k->key_type = prototype_element->v1;
        if (k->key_type < 0 || k->key_type > 3)
        {
            printf(u8"Error: Niepodano typu klucza Y:%i X:%i, błędna mapa!!!\n", y, x);
            exit(1);
        }
        char pathToFile[defaultCharTabLength];
        sprintf(pathToFile, "%s%s%i%s", folderPathDynamic, "key_", k->key_type, ".png");
        k->smalImagePath = (char *)malloc(sizeof(char) * (strlen(pathToFile) + 1));
        strcpy(k->smalImagePath, pathToFile);
        element->image = gtk_image_new_from_file(pathToFile);

        element->objectData = (void *)k;
    }
    else
    {
        if (element != NULL)
            free(element);
        element = NULL;
    }

    return element;
}