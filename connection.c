#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "map_loader.h"
#include "settings.h"

//Ustawienia postaci servera
extern char *characterNameServer;
extern char *characterImagePathServer;
extern int characterServerIndexX;
extern int characterServerIndexY;

//Ustawienia postaci hosta
extern char *characterNameHost;
extern char *characterImagePathHost;
extern int characterHostIndexX;
extern int characterHostIndexY;

void set_characters_index(char pathToMap[])
{

    Prototype_map *pr_map = prototype_load_map(pathToMap);
    int tabX[2];
    int tabY[2]; //Indexy postaci

    int b = 0;
    for (int i = 0; i < pr_map->Y; i++)
    {
        for (int q = 0; q < pr_map->X; q++)
        {
            if (!strcmp((pr_map->map + i * pr_map->X + q)->type_of_object, "ch"))
            {
                if (b < 2)
                {
                    tabX[b] = q;
                    tabY[b] = i;
                }
                b++;
            }
        }
    }

    if (b != 2)
    {
        printf(u8"Error: Niepoprawny plik z mapą, błędna ilość graczy!!!\n");
        exit(1);
    }
    characterServerIndexX=tabX[0];
    characterServerIndexY=tabY[0];

    characterHostIndexX=tabX[1];
    characterHostIndexY=tabY[1];
}

void set_connection(char pathToMap[])
{

    extern bool isAServer;
    isAServer = true;

    extern char *mapPath;
    mapPath = (char *)malloc(sizeof(char) * (strlen(pathToMap) + 1));
    strcpy(mapPath, pathToMap);

    //Ustawienia postaci servera
    characterNameServer = "Lerqiu";
    characterImagePathServer = "./Images/Dynamic/character_server.png";

    //Ustawienia postaci hosta
    characterNameHost = "Berqiu";
    characterImagePathHost = "./Images/Dynamic/character_host.png";

    set_characters_index(pathToMap);
}