#ifndef connection
#define connection

#include "map_loader.h"

void set_connection(char *Nick, bool isServerr, bool isKeyRevert, char *Map,GtkWidget *window);
bool isCheckingPipe(void);
void set_characters_index(char pathToMap[]);

#endif