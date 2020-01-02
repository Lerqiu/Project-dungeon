#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "map_loader.h"
#include "battleground.h"
#include "settings.h"


extern int DEF_IMAGE_SIZE ;
extern char folderPathImages[];     //koniecznie zakończona ukośnikiem
extern char folderPathDynamic[];
extern int maxLengthOfPath;

static void print_prototype_map(Prototype_map *pr_map)
{
    if (pr_map == NULL)
        return;

    int b = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            printf("%s %i %i\t\t", (pr_map->map + i * pr_map->X + q)->type_of_object, (pr_map->map + i * pr_map->X + q)->v1, (pr_map->map + i * pr_map->X + q)->v2);
        }
        putchar('\n');
    }
}

Prototype_map *prototype_load_map(char name[])
{

    FILE *map = fopen(name, "r");
    if (map == NULL)
    {
        printf("Error: Brak pliku %s\n", name);
        return NULL;
    }

    int X, Y;
    fscanf(map, "%i %i", &X, &Y);

    Prototype_map *board = (Prototype_map *)malloc(sizeof(Prototype_map));
    board->X = X;
    board->Y = Y;

    board->map = (Prototype_map_element *)malloc(sizeof(Prototype_map_element) * X * Y);

    char buffer[100];
    int index = 0;
    while (fscanf(map, "%s", buffer) != EOF)
    {
        char buffor_temporary[100];
        int index_of_buffer = 0;
        while (index_of_buffer < strlen(buffer) && buffer[index_of_buffer] != ':' && buffer[index_of_buffer] != '\0' && buffer[index_of_buffer] != '\n' && buffer[index_of_buffer] != EOF)
        {
            buffor_temporary[index_of_buffer] = buffer[index_of_buffer];
            index_of_buffer++;
        }
        buffor_temporary[index_of_buffer] = '\0';
        index_of_buffer++;

        (board->map + index)->type_of_object = (char *)malloc(sizeof(char) * (strlen(buffor_temporary + 1)));
        strcpy((board->map + index)->type_of_object, buffor_temporary);

        strcpy(buffor_temporary, "");
        int index_of_buffor_temporary = 0;
        while (index_of_buffer < strlen(buffer) && buffer[index_of_buffer] != '\0' && buffer[index_of_buffer] != '\n' && buffer[index_of_buffer] != EOF)
        {
            buffor_temporary[index_of_buffor_temporary] = buffer[index_of_buffer];
            index_of_buffer++;
            index_of_buffor_temporary++;
        }
        buffor_temporary[index_of_buffor_temporary] = '\0';

        (board->map + index)->v1 = -1;
        (board->map + index)->v2 = -1;
        sscanf(buffor_temporary, "%i:%i", &((board->map + index)->v1), &((board->map + index)->v2));

        index++;
    }

    fclose(map);
    return board;
}

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

/////////////////////////////////////////////////////////////////////////////
//Elementy statyczne
void map_create_wall(int x, int y, BattlegroundStatic_element *element)
{
    element->topimage = NULL;
    element->downimage = NULL;
    element->type = (char *)malloc(sizeof(char) * (strlen("wall") + 1));
    strcpy(element->type, "wall");
    element->height = DEF_IMAGE_SIZE;
    element->with = DEF_IMAGE_SIZE;
    element->posX = x * DEF_IMAGE_SIZE;
    element->posY = y * DEF_IMAGE_SIZE;
    element->colisionCheckX = DEF_IMAGE_SIZE / 2;
    element->colisionCheckY = DEF_IMAGE_SIZE / 2;
    element->pivotPosX = element->posX + element->posX;
    element->pivotPosY = element->posY + element->posY;
}

void map_create_nothing(int x, int y, BattlegroundStatic_element *element)
{
    element->topimage = NULL;
    element->downimage = NULL;
    element->type = (char *)malloc(sizeof(char) * (strlen("nothing") + 1));
    strcpy(element->type, "nothing");
    element->height = DEF_IMAGE_SIZE;
    element->topimage = NULL;
    element->downimage = NULL;
    element->with = DEF_IMAGE_SIZE;
    element->posX = x * DEF_IMAGE_SIZE;
    element->posY = y * DEF_IMAGE_SIZE;
    element->colisionCheckX = DEF_IMAGE_SIZE / 2;
    element->colisionCheckY = DEF_IMAGE_SIZE / 2;
    element->pivotPosX = element->posX + element->posX;
    element->pivotPosY = element->posY + element->posY;
}

void check_walls(int x, int y, Prototype_map *pr_map, bool *top, bool *right, bool *bootom, bool *left)
{
    if (x > 0)
        if (!strcmp((pr_map->map + y * pr_map->X + x - 1)->type_of_object, "w"))
            *left = true;

    if (x < pr_map->X)
        if (!strcmp((pr_map->map + y * pr_map->X + x + 1)->type_of_object, "w"))
            *right = true;

    if (y > 0)
        if (!strcmp((pr_map->map + (y - 1) * pr_map->X + x)->type_of_object, "w"))
            *top = true;

    if (y < pr_map->Y)
        if (!strcmp((pr_map->map + (y + 1) * pr_map->X + x)->type_of_object, "w"))
            *bootom = true;
}

void check_walls_corners(int x, int y, Prototype_map *pr_map, bool *leftBottom, bool *rightBottom)
{
    if (y < pr_map->Y)
    {
        if (x > 0)
            if (!strcmp((pr_map->map + (y + 1) * pr_map->X + x - 1)->type_of_object, "w"))
                *leftBottom = true;

        if (x < pr_map->X)
            if (!strcmp((pr_map->map + (y + 1) * pr_map->X + x + 1)->type_of_object, "w"))
                *rightBottom = true;
    }
}

static void set_wall(BattlegroundStatic_element *element, char nameImageDown[], char nameImageTop[], int width, int height, int x, int y)
{
    if (strlen(nameImageDown) > 0)
    {
        element->downimage = gtk_image_new_from_file(nameImageDown);
    }
    else
    {
        element->downimage = NULL;
    }

    if (strlen(nameImageTop) > 0)
    {
        element->topimage = gtk_image_new_from_file(nameImageTop);
    }
    else
    {
        element->topimage = NULL;
    }

    element->height = height;
    element->with = width;
    element->type = (char *)malloc(sizeof(char) * (strlen("path") + 1));
    strcpy(element->type, "path");

    element->pivotPosY = height - DEF_IMAGE_SIZE / 2;
    element->pivotPosX = width - DEF_IMAGE_SIZE / 2;

    element->colisionCheckY = DEF_IMAGE_SIZE / 2;
    element->colisionCheckX = DEF_IMAGE_SIZE / 2;

    element->posX = x;
    element->posY = y;
}

void path_add_corner(int x, int y, Prototype_map *pr_map, BattlegroundStatic_element *element, bool *left, bool *bottom, bool *right)
{
    bool leftBottom = false, rightBottom = false;
    x /= DEF_IMAGE_SIZE;
    y /= DEF_IMAGE_SIZE;
    check_walls_corners(x, y, pr_map, &leftBottom, &rightBottom);

    char pathToFile[maxLengthOfPath];

    if (element->topimage == NULL && *bottom == false)
    {
        if (element->height > DEF_IMAGE_SIZE)
        {
            if (leftBottom == true && rightBottom == true && *left == false && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_right_left_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (leftBottom == true && *left == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_left_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (rightBottom == true && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_right_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
        }
        else
        {
            if (leftBottom == true && rightBottom == true && *left == false && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_right_left_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (leftBottom == true && *left == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_left_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (rightBottom == true && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "patch_right_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
        }
    }
}

void map_create_path(int x, int y, Prototype_map *pr_map, BattlegroundStatic_element *element)
{
    bool top = false, right = false, bootom = false, left = false, leftBottom = false, rightBottom = true;
    check_walls(x, y, pr_map, &top, &right, &bootom, &left);

    x *= DEF_IMAGE_SIZE;
    y *= DEF_IMAGE_SIZE;

    char pathToFileDown[maxLengthOfPath];
    char pathToFileTop[maxLengthOfPath];

    if (top == false && right == false && bootom == false && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == false && bootom == false && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_left.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == false && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_left_right.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == false && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_right.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == true && right == false && bootom == false && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == false && bootom == false && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_left.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == true && bootom == false && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_left_right.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == true && bootom == false && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_right.png");
        set_wall(element, pathToFileDown, "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == false && right == false && bootom == true && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == false && bootom == true && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_left.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == true && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_left_right.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == true && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_right.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == true && right == false && bootom == true && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_b.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == false && bootom == true && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_left.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_left_b.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == true && bootom == true && left == true)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_left_right.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_left_right_b.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }
    else if (top == true && right == true && bootom == true && left == false)
    {
        sprintf(pathToFileDown, "%s%s", folderPathImages, "path_wall_top_right.png");
        sprintf(pathToFileTop, "%s%s", folderPathImages, "wall_down_right_b.png");
        set_wall(element, pathToFileDown, pathToFileTop, DEF_IMAGE_SIZE, DEF_IMAGE_SIZE * 2, x, y - DEF_IMAGE_SIZE);
    }

    path_add_corner(x, y, pr_map, element, &left, &bootom, &right);
}

BattlegroundStatic_element *load_battleground_static(Prototype_map_element *prototype_element, int x, int y, Prototype_map *pr_map) //x i y to numery indeksu
{
    BattlegroundStatic_element *element = (BattlegroundStatic_element *)malloc(sizeof(BattlegroundStatic_element));

    if (!strcmp(prototype_element->type_of_object, "w"))
    {
        map_create_wall(x, y, element);
    }
    else if (!strcmp(prototype_element->type_of_object, "n"))
    {
        map_create_nothing(x, y, element);
    }
    else
    {
        map_create_path(x, y, pr_map, element);
    }

    return element;
}

//////////////////////////////////////////////////////////////////////////////////////
//Elementy dynamiczne

void map_create_std_dynamic(int x, int y, BattlegroundDynamic_element *element, char type[])
{
    char pathToFile[maxLengthOfPath];
    sprintf(pathToFile, "%s%s%s", folderPathDynamic, type, ".png");

    if (strcmp(type, "character"))
        element->image = gtk_image_new_from_file(pathToFile);
    else
        element->image = NULL;
    element->height = DEF_IMAGE_SIZE;
    element->with = DEF_IMAGE_SIZE;
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

    element->actionRange = 0;
    element->speed = 0;

    element->objectData = NULL;
    element->viewData = NULL;
}

void set_character_image(BattlegroundDynamic_element *element,char path[]){


}


BattlegroundDynamic_element *load_battleground_dynamic(Prototype_map_element *prototype_element, int x, int y) //x i y to numery indeksu
{
    BattlegroundDynamic_element *element = (BattlegroundDynamic_element *)malloc(sizeof(BattlegroundDynamic_element));

    if (!strcmp(prototype_element->type_of_object, "e"))
    {
        map_create_std_dynamic(x, y, element, "princes");
    }
    else if (!strcmp(prototype_element->type_of_object, "ch"))
    {
        map_create_std_dynamic(x, y, element, "character");
    }
    else if (!strcmp(prototype_element->type_of_object, "g"))
    {
        map_create_std_dynamic(x, y, element, "gate");
    }
    else if (!strcmp(prototype_element->type_of_object, "t"))
    {
        map_create_std_dynamic(x, y, element, "trap");
    }
    else if (!strcmp(prototype_element->type_of_object, "m"))
    {
        map_create_std_dynamic(x, y, element, "monster");
    }
    else if (!strcmp(prototype_element->type_of_object, "k"))
    {
        map_create_std_dynamic(x, y, element, "key");
    }
    else
    {
        free(element);
        element = NULL;
    }

    return element;
}