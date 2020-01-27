#ifndef Battleground_add_object_to_window
#define Battleground_add_object_to_window

#include <gtk/gtk.h>

#include "Map_loader_prototype.h"

void create_battleground_static(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay);
void create_battleground__static_top(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay);
void create_battleground_dynamic(GtkWidget *window, Prototype_map *pr_map, GtkWidget *lay,GtkAdjustment *hadjCharacter ,GtkAdjustment *vadjCharacter );

#endif