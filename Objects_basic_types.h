#ifndef Objects_basic_types
#define Objects_basic_types

#include <gtk/gtk.h>
#include <stdbool.h>
//Static

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

//////Dynamic

typedef struct monster_data
{
    int direction;
    int indexOfFrame;
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
    GtkWidget *viewData;
    //void *viewData; //Implementacje animacjii póżniej

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

//Znajdowanie obiektu

typedef struct
{
    int index;
    BattlegroundDynamic_element *pointer;
} Pointer_and_Index;


BattlegroundDynamic *getObjectByType(char type[]);
Pointer_and_Index *getObject_by_ids(int indexY, int indexX);
void delete_BattlegroundDynamic_element(int indexY, int indexX);

#endif