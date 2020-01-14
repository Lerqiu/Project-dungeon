#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "map_loader.h"
#include "settings.h"
#include "fifo.h"
#include "battleground.h"
#include "window_creator.h"

//Potrzebne w setConnection
extern bool isServer;
extern char *mapPath;
extern bool reverseKeyBoard;
extern char folderPathMaps[];
extern char *mapPath;
extern int maxLengthOfPath;

extern int timeForConnectionCheck;

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
extern int mapRows;
extern int mapColumns;

void set_characters_index(Prototype_map *pr_map)
{
    mapRows = pr_map->X;
    mapColumns = pr_map->Y;

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
    characterServerIndexX = tabX[0];
    characterServerIndexY = tabY[0];

    characterHostIndexX = tabX[1];
    characterHostIndexY = tabY[1];
}

int amount_of_checks; //= timeForConnectionCheck;
GtkWidget *dialPointer = NULL;
bool isChecking = false;

static gboolean wait_for_start_signal_end(void)
{
    if (dialPointer != NULL)
        gtk_widget_destroy(dialPointer);
    isChecking = false;
    amount_of_checks = timeForConnectionCheck;
    dialPointer = NULL;
    return FALSE;
}

static gboolean check_start_signal(void)
{
    char buffer[1000];
    int size = 1000;
    gint64 time = g_get_real_time();
    gint64 returnTime;

    while (getStringFromPipe(buffer, size) == true)
    {

        int a = sscanf(buffer, "Ready %li\n", &returnTime);
        if (a == 1)
        {
            // printf("Poprawnie wczytany czas:%li\n", returnTime);
            if (labs(returnTime - time) < 30 * 1000000)
            {
                getStringFromPipe(buffer, size);
                //printf("%s", buffer);
                int isServerOther;
                sscanf(buffer, "isServer %i\n", &isServerOther);
                int n; //= 1 ? 0 : isServer==true;
                if (isServer == true)
                    n = 1;
                else
                    n = 0;

                //printf("n:%i\n", n);
                //printf("isServerOther %i\n", isServerOther);
                if (n != isServerOther)
                {
                    //printf("Poprawnie wczytano typ server/host:%i\n", isServerOther);
                    if (isServerOther == 1)
                    {
                        getStringFromPipe(buffer, size);
                        // printf("%s", buffer);
                        char *mapOther = (char *)malloc(sizeof(char) * maxLengthOfPath);
                        sscanf(buffer, "Map %s\n", mapOther);
                        //printf("Map:%s\n", mapOther);
                        mapPath = mapOther;
                        //printf("Poprawnie wczytano mapę:%s\n", mapPath);
                    }
                    getStringFromPipe(buffer, size);
                    //printf("%s", buffer);
                    char *nickOther;
                    sscanf(buffer, "Nick %s\n", nickOther);
                    //printf("Nick:%s\n", nickOther);
                    if (isServerOther == 1)
                    {
                        characterNameServer = nickOther;
                    }
                    else
                    {
                        characterNameHost = nickOther;
                    }
                    //printf("Poprawnie wczytano nick:%s\n", nickOther);
                    getStringFromPipe(buffer, size);
                    //printf("%s", buffer);
                    int isKeyBoR;
                    sscanf(buffer, "isKeyBoR %i\n", &isKeyBoR);
                    int m; //= 1 ? 0 : reverseKeyBoard;
                    if (reverseKeyBoard == true)
                        m = 1;
                    else
                        m = 0;
                    //printf("Poprawnie wczytano keyR:%i\n", isKeyBoR);
                    if (m != isKeyBoR)
                    {
                        wait_for_start_signal_end();
                        //gtk_main_quit();
                        //destroyWindowSetSoft();
                        destroyLocalWindow();
                        gtk_main_iteration_do (FALSE);
                        create_battleground(mapPath);

                        return FALSE;
                    }
                }
            }
        }

        a = 0;
    }

    return TRUE;
}

static gboolean wait_for_start_signal(gpointer data)
{
    GtkWidget *window = (GtkWidget *)data;
    if (dialPointer != NULL)
        gtk_widget_destroy(dialPointer);
    dialPointer = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "Sprawdzanie połączenia (trwa %i s). Pozostało: %i", timeForConnectionCheck, amount_of_checks);
    gtk_widget_show(dialPointer);

    amount_of_checks--;
    if (amount_of_checks < 0)
    {
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Wprowadzono niepoprawne dane lub minął czas połączenia\nSprawdz czy:\n-Ustawiono odmienne tryby gry\n-Ustawiono odmienne tryby klawiatury");
        gtk_dialog_run(GTK_DIALOG(d));
        gtk_widget_destroy(d);
        return wait_for_start_signal_end();
    }

    return check_start_signal();
}

static void send_start_signal(char Nick[], GtkWidget *window)
{
    //gtk_layout_new(NULL,NULL);
    gint64 time = g_get_real_time();

    char buffer[maxLengthOfPath];
    sprintf(buffer, "Ready %li\n", time);
    //printf("%s", buffer);
    sendStringToPipe(buffer);

    if (isServer)
    {
        sprintf(buffer, "isServer 1\n");
    }
    else
    {
        sprintf(buffer, "isServer 0\n");
    }
    //printf("%s", buffer);
    sendStringToPipe(buffer);

    if (isServer)
    {
        sprintf(buffer, "Map %s\n", mapPath);
        //printf("%s", buffer);
        sendStringToPipe(buffer);
    }

    sprintf(buffer, "Nick %s\n", Nick);
    //printf("%s", buffer);
    sendStringToPipe(buffer);

    if (reverseKeyBoard)
    {
        sprintf(buffer, "isKeyBoR 1\n");
    }
    else
    {
        sprintf(buffer, "isKeyBoR 0\n");
    }
    //printf("%s", buffer);
    //printf("Dane wysłane koniec\n");
    sendStringToPipe(buffer);
    g_timeout_add(1000, wait_for_start_signal, window);
}

static void createMap()
{
}

bool isCheckingPipe(void)
{
    return isChecking;
}

void set_connection(char *Nick, bool isServerr, bool isKeyRevert, char *Map, GtkWidget *window)
{
    closePipes();
    isChecking = true;

    amount_of_checks = timeForConnectionCheck;

    isServer = isServerr;

    reverseKeyBoard = isKeyRevert;

    mapPath = FullName_Path_get(folderPathMaps, Map);

    if (isServer)
    {
        characterNameServer = Nick;
        initPipes("A");
    }
    else
    {
        characterNameHost = Nick;
        initPipes("B");
    }

    send_start_signal(Nick, window);

    //set_characters_index(FullName_Path_get(folderPathMaps, mapPath));
}