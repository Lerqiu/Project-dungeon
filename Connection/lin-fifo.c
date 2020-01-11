#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <fcntl.h>
#include "fifo.h"

struct pipes {
    FILE *fifo_in, *fifo_out;
    int isA;
} ;


#define MAKS_DL_TEKSTU 10000

static GtkWidget *window, *bufor;
static PipesPtr potoki;
static char *moj_id, *twoj_id;

static void przekaz_tekst( GtkWidget *widget,GtkWidget *text);
static gboolean pobierz_tekst(gpointer data);
static void zakoncz(GtkWidget *widget, gpointer data);

int fileno(FILE *file);
void pokazBlad(char *komunikat);
static FILE *openOutPipe(char *name);
static FILE *openInPipe(char *name);

void closePipes(PipesPtr pipes) 
{ 
  fclose(pipes->fifo_in); 
  fclose(pipes->fifo_out); 
  free(pipes); 
}

PipesPtr initPipes(char argv[])
{
    if ((argv[0] != 'A' && argv[0] != 'B'))
    {
        fprintf(stderr,"\nThis program should be called with the first argument: A or B\n\n");
	mkfifo("AtoB",0664);
	mkfifo("BtoA",0664);
	fprintf(stderr,"Fifo queues AtoB and BtoA created\n");
        return NULL;
    }
    PipesPtr pipes=(PipesPtr)malloc(sizeof(struct pipes));
    if (pipes == NULL) {
        fprintf(stderr,"Memory allocation error\n");
    } else {
        pipes->isA=(argv[0] == 'A');
        pipes->fifo_out = openOutPipe(pipes->isA ? "AtoB" : "BtoA");
        pipes->fifo_in = openInPipe(pipes->isA ? "BtoA" : "AtoB");
    }
    return pipes;
}

static FILE *openOutPipe(char *name) {
    mkfifo(name,0664);
    FILE *pipe = fopen(name, "w+");
    if (pipe == NULL)
    {
        pokazBlad("Error in creating output pipe");
        exit(-1);
    }
    return pipe;
}

static FILE *openInPipe(char *name){
    FILE *pipe = fopen(name, "r+");
    if (pipe == NULL) {
        mkfifo(name,0664);
        pipe = fopen(name, "r+");
    }
    if (pipe == NULL)
    {
        pokazBlad("Error in creating input pipe");
        exit(-1);
    }
    int flags, fd;
    fd = fileno(pipe);
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return pipe;
}

void sendStringToPipe(PipesPtr pipes, const char *data)
{
    int result = fprintf(pipes->fifo_out,"%s",data);
    fflush(pipes->fifo_out);
    if (result == 0)
        pokazBlad("Failed to send data");
}

bool getStringFromPipe(PipesPtr pipes, char *buffer, size_t size)
{
    char *result = fgets(buffer,size,pipes->fifo_in);
    fflush(pipes->fifo_in);
    //if (result == NULL) pokazBlad("Failed to read data");
    return result != NULL;
}

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

int setPipe(GtkWidget *window,char argv[])
{
    if(argv == NULL || window == NULL)
        return 1;
    if(argv[0]!='A' && argv[0]!='B' ){
        printf("Error: Niepoprawny parametr 2 funkcja:setPipe\n");
        return 1;
    }

    if ((potoki=initPipes(argv)) == NULL)
        return 1;
    if (strcmp(argv[0],"A") == 0) { twoj_id="B > "; moj_id="A > "; }
    else { moj_id="B > "; twoj_id="A > "; }


    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(zakoncz), NULL);

    return 0;
}

static void przekaz_tekst( GtkWidget *widget,GtkWidget *text)
{
    gchar wejscie[MAKS_DL_TEKSTU+5];

    sendStringToPipe(potoki, gtk_entry_get_text (GTK_ENTRY (text)));

    strcpy(wejscie,moj_id);
    strcpy(wejscie+strlen(wejscie),gtk_entry_get_text (GTK_ENTRY (text)));
    strcat(wejscie,"\n");

    gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(bufor),wejscie,-1);
    gtk_entry_set_text(GTK_ENTRY(text), "");

}

static gboolean pobierz_tekst(gpointer data)
{
  gchar wejscie[MAKS_DL_TEKSTU+5];

  strcpy(wejscie,twoj_id);
  if (getStringFromPipe(potoki,wejscie+strlen(wejscie),MAKS_DL_TEKSTU)) {
      strcat(wejscie,"\n");
      gtk_text_buffer_insert_at_cursor (GTK_TEXT_BUFFER(bufor),wejscie,-1);
  }
  return TRUE;
}

static void zakoncz(GtkWidget *widget, gpointer data)
{
  closePipes(potoki);
  gtk_main_quit();
}
