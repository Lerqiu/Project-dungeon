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
char folderPathMaps[] = "./Maps/";              //koniecznie zakończona ukośnikiem
char folderPathOthers[] = "./Images/Others/";
int windowWidth = 64 * 11;
int windowHeight = 64 * 7;
char gameName[] = "Save the princes ";
int timeForConnectionCheck = 30;//w sek

//Ustawienia gry
bool isServer = true;
bool reverseKeyBoard = false;
char *mapPath = "./Maps/Dungeon1.txt";
int defaultMonsterSpeed = 13;
int defaultCharacterSpeed = 10;
int defaultCharacterActionRangePx = 64 / 2;
long long int loopTime;
int mapRows;
int mapColumns;

//Ustawienia postaci servera
char *characterNameServer = NULL;
char  *characterImagePathServer = "./Images/Dynamic/character_server.png";
int characterServerIndexX;
int characterServerIndexY;

//Ustawienia postaci hosta
char *characterNameHost = NULL;
char *characterImagePathHost = "./Images/Dynamic/character_host.png";
int characterHostIndexX;
int characterHostIndexY;

//Potoki
//A <- server
//B <- host

char *FullName_Path_get(char a[], char b[])
{
    char *buffer = (char *)malloc(sizeof(char) * (maxLengthOfPath));
    sprintf(buffer, "%s%s", a, b);
    return buffer;
}