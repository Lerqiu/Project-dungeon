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

//Ustawienia gry
bool isAServer=true;
char *mapPath=NULL;
int defaultMonsterSpeed=13;
int defaultCharacterSpeed=10;

//Ustawienia postaci servera
char *characterNameServer=NULL;
char *characterImagePathServer=NULL;
int characterServerIndexX;
int characterServerIndexY;

//Ustawienia postaci hosta
char *characterNameHost=NULL;
char *characterImagePathHost=NULL;
int characterHostIndexX;
int characterHostIndexY;

