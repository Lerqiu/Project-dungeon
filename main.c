#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "window_creator.h"
#include <gtk/gtk.h>

int main(void)
{
    gtk_init(NULL, NULL);
    time_t tt;
    srand(time(&tt));
    createStartWindow();

    return 0;
}