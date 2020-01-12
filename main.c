#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_loader.h"
#include "connection.h"
#include "window_creator.h"
#include "fifo.h"
#include <gtk/gtk.h>

int main(void)
{
    createStartWindow();

    if (isConnectedPipe() == true)
    {
        //set_connection("./Maps/Dungeon1.txt");
        //create_battleground("./Maps/Dungeon1.txt");
    }
    //set_connection("./Maps/Dungeon1.txt");
    //create_battleground("./Maps/Dungeon1.txt");

    //closePipes();//Zamknięcie potoku init w setConnection

    return 1;
}