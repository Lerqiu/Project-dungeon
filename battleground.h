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

void create_battleground(char mapPath[]);
void destroy_window(GtkWidget *window);


typedef struct monster_data{

int startX,startY;
int endX,endY;
int direction;

}monsterData;


typedef struct battlegrounddynamic_element
{
    char *type;
    int indexStartPointX,indexStartPointY;//unikalny indeks 

    GtkWidget *image;
    void *viewData;//Implementacje animacjii póżniej

    int with,height;
    int posX,posY;
    int pivotPosX,pivotPosY;
    //wzgledem posX i posY
    int colisionCheckX,colisionCheckY;
    //Kolizja liczona od posPivot, obszar prostokąta

    int actionRange;//Np. atak potworów, czy otwieranie bramy kluczem
    int speed;//Prętkość poruszania się

    void *objectData;//Dodatkowe informacje o obiekcie
} BattlegroundDynamic_element;

typedef struct battlegroundDynamic
{
    BattlegroundDynamic_element **tabOfElements;
    int amount;
} BattlegroundDynamic;

#endif