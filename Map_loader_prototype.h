#ifndef Map_loader_prototype
#define Map_loader_prototype


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

#endif