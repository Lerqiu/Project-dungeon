#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdbool.h>

#include "settings.h"

//Ustawienia ogólne
int DEF_IMAGE_SIZE = 64;
int maxLengthOfPath = 300;
char folderPathImages[] = "./Images/Path/";     //koniecznie zakończona ukośnikiem
char folderPathDynamic[] = "./Images/Dynamic/"; //koniecznie zakończona ukośnikiem
int windowWidth=64*11;
int windowHeight=64*7;

//Ustawienia gry
bool isServer = true;
bool reverseKeyBoard = false;
char *mapPath = NULL;
int defaultMonsterSpeed = 13;
int defaultCharacterSpeed = 10;
int defaultCharacterActionRangePx=64/2;
long long int loopTime;
int mapRows;
int mapColumns;

//Ustawienia postaci servera
char *characterNameServer = NULL;
char *characterImagePathServer = NULL;
int characterServerIndexX;
int characterServerIndexY;

//Ustawienia postaci hosta
char *characterNameHost = NULL;
char *characterImagePathHost = NULL;
int characterHostIndexX;
int characterHostIndexY;

GtkAdjustment *hadj=NULL;
GtkAdjustment *vadj=NULL;