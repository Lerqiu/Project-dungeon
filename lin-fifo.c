#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <fcntl.h>

#include "fifo.h"

struct pipes
{
    FILE *fifo_in, *fifo_out;
    int isA;
};

#define MAKS_DL_TEKSTU 10000

static PipesPtr potoki = NULL;

int fileno(FILE *file);
static FILE *openOutPipe(char *name);
static FILE *openInPipe(char *name);

void closePipes(void)
{
    PipesPtr pipes = potoki;
    if (potoki == NULL)
        return;
    fclose(pipes->fifo_in);
    fclose(pipes->fifo_out);
    free(potoki);
    potoki = NULL;
}

void initPipes(char argv[])
{
    if ((argv[0] != 'A' && argv[0] != 'B'))
    {
        fprintf(stderr, "\nThis program should be called with the first argument: A or B\n\n");
        mkfifo("AtoB", 0664);
        mkfifo("BtoA", 0664);
        fprintf(stderr, "Fifo queues AtoB and BtoA created\n");
        return;
    }
    PipesPtr pipes = (PipesPtr)malloc(sizeof(struct pipes));
    if (pipes == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
    }
    else
    {
        pipes->isA = (argv[0] == 'A');
        pipes->fifo_out = openOutPipe(pipes->isA ? "AtoB" : "BtoA");
        pipes->fifo_in = openInPipe(pipes->isA ? "BtoA" : "AtoB");
    }
    potoki = pipes;
}

static FILE *openOutPipe(char *name)
{
    mkfifo(name, 0664);
    FILE *pipe = fopen(name, "w+");
    if (pipe == NULL)
    {
        printf("Error in creating output pipe\n");
        exit(-1);
    }
    return pipe;
}

static FILE *openInPipe(char *name)
{
    FILE *pipe = fopen(name, "r+");
    if (pipe == NULL)
    {
        mkfifo(name, 0664);
        pipe = fopen(name, "r+");
    }
    if (pipe == NULL)
    {
        printf("Error in creating input pipe\n");
        exit(-1);
    }
    int flags, fd;
    fd = fileno(pipe);
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return pipe;
}

void sendStringToPipe(const char *data)
{
    if (data == NULL)
        return;

    PipesPtr pipes = potoki;
    int result = fprintf(pipes->fifo_out, "%s", data);
    fflush(pipes->fifo_out);
    if (result == 0)
        printf("Failed to send data\n");

    //printf("Tekst wysłany: %s", data);
}

bool getStringFromPipe(char *buffer, size_t size)
{
    if (buffer == NULL)
        return false;

    PipesPtr pipes = potoki;
    char *result = fgets(buffer, size, pipes->fifo_in);
    fflush(pipes->fifo_in);

    //printf("Tekst odebrany: %s", buffer);
    //if(strlen(buffer)<1)
    // putchar('\n');

    return result != NULL;
}

bool isConnectedPipe(void)
{
    if (potoki == NULL)
        return false;
    return true;
}