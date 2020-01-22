#ifndef battleground
#define battleground

#include <gtk/gtk.h>
#include <stdbool.h>

typedef struct battlegroundstatic_element
{
    GtkWidget *downimage;
    GtkWidget *topimage;
    int width, height;
    int posX, posY;
    int pivotPosX, pivotPosY;
    //wzgledem posX i posY
    int colisionCheckX, colisionCheckY;
    //Kolizja liczona od posPivot, obszar prostokąta
    char *type;
} BattlegroundStatic_element;

typedef struct battlegroundStatic
{
    BattlegroundStatic_element **map;
    int X, Y;
} BattlegroundStatic;

void create_battleground();
void destroy_window(GtkWidget *window);

typedef struct monster_data
{
    int direction;
    int amountsOfSteps;
    int endIndexX;
    int endIndexY;
    bool isColision;

} MonsterData;

typedef struct character_data
{

    GtkAdjustment *hadj;
    GtkAdjustment *vadj;
    int keyTab[4];//typy kluczy

} CharacterData;

typedef struct gate_data
{

    bool isOpen;
    int key_type;

} GateData;

typedef struct key_data
{
    int key_type;
    char *smalImagePath;

} KeyData;

typedef struct battlegrounddynamic_element
{
    char *type;
    int indexStartPointX, indexStartPointY; //unikalny indeks

    GtkWidget *image;
    void *viewData; //Implementacje animacjii póżniej

    GtkWidget *layout;

    int width, height;
    int posX, posY;
    int pivotPosX, pivotPosY;
    //wzgledem posX i posY
    int colisionCheckX, colisionCheckY;
    //Kolizja liczona od posPivot, obszar prostokąta

    int actionRange; //Np. atak potworów, czy otwieranie bramy kluczem
    int speed;       //Prętkość poruszania się

    void *objectData; //Dodatkowe informacje o obiekcie
} BattlegroundDynamic_element;

typedef struct battlegroundDynamic
{
    BattlegroundDynamic_element **tabOfElements;
    int amount;
} BattlegroundDynamic;

#endif