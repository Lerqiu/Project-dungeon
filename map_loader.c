#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_loader.h"
#include "battleground.h"

#include <stdbool.h>

//#define DEF_IMAGE_SIZE 64
DEF_IMAGE_SIZE=64;

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

static void set_wall(BattlegroundStatic_element *element, char nameImageDown[], char nameImageTop[], int width, int height, int x, int y)
{
    if (strlen(nameImageDown) > 0)
    {
        //element->downimage = (char *)malloc(sizeof(char) * (strlen(nameImageDown) + 1));
        //strcpy(element->downimage, nameImageDown);
        element->downimage = gtk_image_new_from_file(nameImageDown);
    }
    else
    {
        element->downimage = NULL;
    }

    if (strlen(nameImageTop) > 0)
    {
        //element->topimage = (char *)malloc(sizeof(char) * (strlen(nameImageTop) + 1));
        //strcpy(element->topimage, nameImageTop);
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

void map_create_path(int x, int y, Prototype_map *pr_map, BattlegroundStatic_element *element)
{
    bool top = false, right = false, bootom = false, left = false;
    check_walls(x, y, pr_map, &top, &right, &bootom, &left);

    x*=DEF_IMAGE_SIZE;
    y*=DEF_IMAGE_SIZE;

    if (top == false && right == false && bootom == false && left == false)
    {
        set_wall(element, "./Images/path.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == false && bootom == false && left == true)
    {
        set_wall(element, "./Images/path_wall_left.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == false && left == true)
    {
        set_wall(element, "./Images/path_wall_left_right.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == false && left == false)
    {
        set_wall(element, "./Images/path_wall_right.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == true && right == false && bootom == false && left == false)
    {
        set_wall(element, "./Images/path_wall_top.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
    else if (top == true && right == false && bootom == false && left == true)
    {
        set_wall(element, "./Images/path_wall_top_left.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
    else if (top == true && right ==  true && bootom == false && left == true)
    {
        set_wall(element, "./Images/path_wall_top_left_right.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
     else if (top == true && right ==  true && bootom == false && left == false)
    {
        set_wall(element, "./Images/path_wall_top_right.png", "", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
    else if (top == false && right == false && bootom == true && left == false)
    {
        set_wall(element, "./Images/path.png", "./Images/wall_down.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == false && bootom == true && left == true)
    {
        set_wall(element, "./Images/path.png", "./Images/wall_down_left.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == true && left == true)
    {
        set_wall(element, "./Images/path.png", "./Images/wall_down_left_right.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == false && right == true && bootom == true && left == false)
    {
        set_wall(element, "./Images/path.png", "./Images/wall_down_right.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE, x, y);
    }
    else if (top == true && right == false && bootom == true && left == false)
    {
        set_wall(element, "./Images/path_wall_top.png", "./Images/wall_down_b.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
    else if (top == true && right == false && bootom == true && left == true)
    {
        set_wall(element, "./Images/path_wall_top_left.png", "./Images/wall_down_left_b.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
    else if (top == true && right ==  true && bootom == true && left == true)
    {
        set_wall(element, "./Images/path_wall_top_left_right.png", "./Images/wall_down_left_right_b.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
     else if (top == true && right ==  true && bootom == true && left == false)
    {
        set_wall(element, "./Images/path_wall_top_right.png", "./Images/wall_down_right_b.png", DEF_IMAGE_SIZE, DEF_IMAGE_SIZE*2, x, y-DEF_IMAGE_SIZE);
    }
}

BattlegroundStatic_element *load_battleground_static(Prototype_map_element *prototype_element, int x, int y, Prototype_map *pr_map)//x i y to numery indeksu
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