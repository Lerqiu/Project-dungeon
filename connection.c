#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

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
extern int maxLengthOfPath;

extern int timeForConnectionCheck;
extern int MainCharacterServerSearchMapIndex;

//Ustawienia postaci servera
extern char *characterNameServer;
extern char *characterImagePathServer;

//Ustawienia postaci hosta
extern char *characterNameHost;
extern char *characterImagePathHost;
extern int mapRows;
extern int mapColumns;

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

int recivedDataColumns = 6;
char recivedData[6][2][300] = {{"Ready", "-1"},
                               {"isServer", "-1"},
                               {"Map", "-1"},
                               {"Nick", "-1"},
                               {"isKeyBoR", "-1"},
                               {"serverCharacterIndexFoundFromMap", "-1"}};

static void resetReciveData(void)
{
    for (int i = 0; i < recivedDataColumns; i++)
    {
        strcpy(recivedData[i][1], "-1");
    }
}

static bool checkRecivedData(void)
{

    for (int i = 0; i < recivedDataColumns; i++)
    {
        if (isServer)
        {
            if (!strcmp(recivedData[i][1], "-1") && (i != 2 && i != 5))
            {
                return false;
            }
        }
        else
        {
            if (!strcmp(recivedData[i][1], "-1"))
            {
                return false;
            }
        }
    }
    gint64 localTime = g_get_real_time();
    gint64 recivedTime;
    if (sscanf(recivedData[0][1], "%li", &recivedTime) < 1)
        return false;

    if (labs(recivedTime - localTime) / 1000000 > timeForConnectionCheck)
    {
        resetReciveData();
        return false;
    }

    bool isServer_RecivedData;
    bool reverseKeyBoard_RecivedData;

    if (!strcmp("1", recivedData[1][1]))
        isServer_RecivedData = true;
    else
        isServer_RecivedData = false;

    if (isServer_RecivedData == isServer)
        return false;

    if (!strcmp("1", recivedData[4][1]))
        reverseKeyBoard_RecivedData = true;
    else
        reverseKeyBoard_RecivedData = false;

    if (reverseKeyBoard == reverseKeyBoard_RecivedData)
        return false;

    if (!isServer)
    {
        if (!strcmp(recivedData[2][1], "-1"))
            return false;
        if (mapPath != NULL)
            free(mapPath);
        mapPath = (char *)malloc(sizeof(char) * maxLengthOfPath);
        strcpy(mapPath, recivedData[2][1]);

        if (mapPath != NULL)
            free(characterNameServer);
        characterNameServer = (char *)malloc(sizeof(char) * maxLengthOfPath);
        strcpy(characterNameServer, recivedData[3][1]);

        if (!strcmp(recivedData[5][1], "-1"))
            return false;

        if (!strcmp("1", recivedData[5][1]))
            MainCharacterServerSearchMapIndex = 1;
        else
            MainCharacterServerSearchMapIndex = 0;
    }
    else
    {
        if (characterNameHost != NULL)
            free(characterNameHost);
        characterNameHost = (char *)malloc(sizeof(char) * maxLengthOfPath);
        strcpy(characterNameHost, recivedData[3][1]);
    }

    return true;
}
bool isReady = false;

static gboolean check_start_signal(void)
{
    int a = 0;
    int buffer_size = 1000;
    char buffer[buffer_size];

    char data_1[maxLengthOfPath];
    char data_2[maxLengthOfPath];

    while (a < 40 && getStringFromPipe(buffer, buffer_size))
    {
        a++;

        if (sscanf(buffer, "%s %s", data_1, data_2) < 2)
            continue;

        for (int i = 0; i < recivedDataColumns; i++)
        {
            if (!strcmp(recivedData[i][0], data_1))
            {
                strcpy(recivedData[i][1], data_2);
                break;
            }
        }
    }

    if (checkRecivedData())
    {

        wait_for_start_signal_end();
        destroyStartWindowContainers();
        if (!isReady)
        {
            isReady = true;
            create_battleground();
        }

        return FALSE;
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
    gint64 time = g_get_real_time();

    char buffer[maxLengthOfPath];
    sprintf(buffer, "%s %li\n", recivedData[0][0], time);
    sendStringToPipe(buffer);

    if (isServer)
    {
        sprintf(buffer, "%s %i\n", recivedData[1][0], 1);
    }
    else
    {
        sprintf(buffer, "%s %i\n", recivedData[1][0], 0);
    }
    sendStringToPipe(buffer);

    if (isServer)
    {
        sprintf(buffer, "%s %s\n", recivedData[2][0], mapPath);
        sendStringToPipe(buffer);
        sprintf(buffer, "%s %i\n", recivedData[5][0], MainCharacterServerSearchMapIndex);
        sendStringToPipe(buffer);
    }

    sprintf(buffer, "%s %s\n", recivedData[3][0], Nick);
    sendStringToPipe(buffer);

    if (reverseKeyBoard)
    {
        sprintf(buffer, "%s %i\n", recivedData[4][0], 1);
    }
    else
    {
        sprintf(buffer, "%s %i\n", recivedData[4][0], 0);
    }
    sendStringToPipe(buffer);

    wait_for_start_signal(window);
    g_timeout_add(1000, wait_for_start_signal, window);
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
        MainCharacterServerSearchMapIndex = rand() % 2;
    }
    else
    {
        characterNameHost = Nick;
        initPipes("B");
    }

    send_start_signal(Nick, window);
}