#ifndef Set_connections_betwen_players
#define Set_connections_betwen_players

#include <gtk/gtk.h>

void set_connection(char *Nick, bool isServerr, bool isKeyRevert, char *Map,GtkWidget *window);
bool isCheckingPipe(void);

#endif