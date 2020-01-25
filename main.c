#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Menu_window_view.h"
#include "Windows_FIFO.h"
#include <gtk/gtk.h>

int main(void)
{
    gtk_init(NULL, NULL);
    time_t tt;
    srand(time(&tt));
    createStartWindowMenu();
    gtk_main();
    closePipes();

    return 0;
}