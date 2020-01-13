#ifndef window_creator
#define window_creator

#include <gtk/gtk.h>


GtkWidget *window_creator_create_window(void);
void createStartWindow(void);
void destroyLocalWindow(void);
void destroyWindowSetSoft(void);

#endif