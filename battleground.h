#ifndef battleground
#define battleground

#include <gtk/gtk.h>

typedef struct battlegroundstatic_element
{
    GtkWidget *downimage;
    GtkWidget *topimage;
    int with,height;
    int posX,posY;
    int pivotPosX,pivotPosY;
    //wzgledem posX i posY
    int colisionCheckX,colisionCheckY;
    //Kolizja liczona od posPivot, obszar prostokąta
    char *type;
} BattlegroundStatic_element;

typedef struct battlegroundStatic
{
    BattlegroundStatic_element **map;
    int X, Y;
} BattlegroundStatic;

void create_battleground(char mapName[]);
void destroy_window(GtkWidget *window);

GtkWidget *pointer_to_main_window;

#endif