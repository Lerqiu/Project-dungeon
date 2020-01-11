#include <stdbool.h>

typedef struct pipes *PipesPtr;

PipesPtr initPipes(char argv[]);
void     sendStringToPipe(PipesPtr channel, const char *data);
bool     getStringFromPipe(PipesPtr channel, char *buffer, size_t size);
void     closePipes(PipesPtr channel);

static void przekaz_tekst( GtkWidget *widget,GtkWidget *text);
static gboolean pobierz_tekst(gpointer data);
static void zakoncz(GtkWidget *widget, gpointer data);

int fileno(FILE *file);
void pokazBlad(char *komunikat);
static FILE *openOutPipe(char *name);
static FILE *openInPipe(char *name);
