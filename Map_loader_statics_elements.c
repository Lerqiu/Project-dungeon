#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Map_loader_statics_elements.h"
#include "Map_loader_prototype.h"
#include "Objects_basic_types.h"
#include "Default_settings.h"

extern int DEF_IMAGE_SIZE;
extern char folderPathImages[]; //koniecznie zakończona ukośnikiem
extern int defaultCharTabLength;


void map_create_wall(int x, int y, BattlegroundStatic_element *element)
{
    element->topimage = NULL;
    element->downimage = NULL;
    element->type = (char *)malloc(sizeof(char) * (strlen("wall") + 1));
    strcpy(element->type, "wall");
    element->height = DEF_IMAGE_SIZE;
    element->width = DEF_IMAGE_SIZE;
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
    element->width = DEF_IMAGE_SIZE;
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
    element->width = width;
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

    char pathToFile[defaultCharTabLength];

    if (element->topimage == NULL && *bottom == false)
    {
        if (element->height > DEF_IMAGE_SIZE)
        {
            if (leftBottom == true && rightBottom == true && *left == false && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_right_left_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (leftBottom == true && *left == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_left_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (rightBottom == true && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_right_bottom_corner_b.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
        }
        else
        {
            if (leftBottom == true && rightBottom == true && *left == false && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_right_left_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (leftBottom == true && *left == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_left_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
            else if (rightBottom == true && *right == false)
            {
                sprintf(pathToFile, "%s%s", folderPathImages, "path_right_bottom_corner.png");
                element->topimage = gtk_image_new_from_file(pathToFile);
            }
        }
    }
}

void map_create_path(int x, int y, Prototype_map *pr_map, BattlegroundStatic_element *element)
{
    bool top = false, right = false, bootom = false, left = false;
    check_walls(x, y, pr_map, &top, &right, &bootom, &left);

    x *= DEF_IMAGE_SIZE;
    y *= DEF_IMAGE_SIZE;

    char pathToFileDown[defaultCharTabLength];
    char pathToFileTop[defaultCharTabLength];

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
