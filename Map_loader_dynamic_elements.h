#ifndef Map_loader_dynamic_elements
#define Map_loader_dynamic_elements


#include <gtk/gtk.h>
#include "Map_loader_prototype.h"

#include "Objects_basic_types.h"

int amount_of_dynamic_elements(Prototype_map *pr_map);
BattlegroundDynamic_element *load_battleground_dynamic(Prototype_map_element *prototype_element, int x, int y); //x i y to numery indeksu


#endif