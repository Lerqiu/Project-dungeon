#ifndef Windows_FIFO
#define Windows_FIFO

#include <stdbool.h>

typedef struct pipes *PipesPtr;

void initPipes(char argv[]);
void sendStringToPipe(const char *data);
bool getStringFromPipe(char *buffer, size_t size);
void closePipes(void);
bool isConnectedPipe(void);


#endif