#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_loader.h"
#include "window_creator.h"
#include <gtk/gtk.h>



int main(void)
{
    /*
    Prototype_map *pr_map = prototype_load_map("Maps/Dungeon1.txt");

    int b = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            printf("%s %i %i\t\t", (pr_map->map + i * pr_map->X + q)->type_of_object, (pr_map->map + i * pr_map->X + q)->v1, (pr_map->map + i * pr_map->X + q)->v2);
        }
        putchar('\n');
    }
*/
    create_battleground("./Maps/Dungeon1.txt");

    return 1;
}