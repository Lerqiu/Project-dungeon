#ifndef map_loader
#define map_loader


#include "battleground.h"

typedef struct prototype_map_element
{
    char *type_of_object;
    int v1, v2;
} Prototype_map_element;

typedef struct prototype_map
{
    Prototype_map_element *map;
    int X, Y;
} Prototype_map;

Prototype_map *prototype_load_map(char name[]);
BattlegroundStatic_element *load_battleground_static(Prototype_map_element *prototype_element, int x, int y, Prototype_map *pr_map);//x i y to numery indeksu

#endif