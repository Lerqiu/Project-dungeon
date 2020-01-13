#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "map_loader.h"
#include "connection.h"
#include "window_creator.h"
#include "fifo.h"
#include <gtk/gtk.h>
#include <unistd.h>

extern char *mapPath;

int main(void)
{
    createStartWindow();
    printf("Buuu\n");
    sleep(5);
    /*
    if (isConnectedPipe() == true)
    {
        //set_connection("./Maps/Dungeon1.txt");
        printf("Data: %s\n", mapPath);
        create_battleground(mapPath);
    }*/
    //set_connection("./Maps/Dungeon1.txt");
    //create_battleground("./Maps/Dungeon1.txt");

    //closePipes();//Zamknięcie potoku init w setConnection

    return 1;
}