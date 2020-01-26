#ifndef Object_character_events
#define Object_character_events

#include <gtk/gtk.h>

#include "Objects_basic_types.h"

void set_view_center_By_Character(void *ob);
gboolean character_movie_keyboard_reverse(GtkWidget *widget, GdkEventKey *event, gpointer *data);
gboolean character_movie_keyboard(GtkWidget *widget, GdkEventKey *event, gpointer *data);

bool characterSavePrinces(BattlegroundDynamic_element *object);

void Synchronization_character_win(BattlegroundDynamic_element *character);
void Synchronization_character_dead(BattlegroundDynamic_element *character);

void Synchronization_move_character(Pointer_and_Index *poi, char event[]);

#endif