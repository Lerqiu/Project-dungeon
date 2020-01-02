#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_loader.h"
#include "connection.h"
#include <gtk/gtk.h>



int main(void)
{
 
    set_connection("./Maps/Dungeon1.txt");
    create_battleground("./Maps/Dungeon1.txt");

    return 1;
}