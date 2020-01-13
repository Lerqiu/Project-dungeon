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
    gtk_init(NULL, NULL);
    createStartWindow();
    
    if (isConnectedPipe() == true)
    {
        create_battleground(mapPath);
    }

    closePipes();//Zamknięcie potoku init w setConnection

    return 1;
}