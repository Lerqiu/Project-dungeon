#ifndef Object_monster_events
#define Object_monster_events

#include <gtk/gtk.h>

#include "Objects_basic_types.h"

gboolean monster_move(gpointer data);
void monsterChangeBodyView(BattlegroundDynamic_element *monster);
void Synchronization_move_monster(Pointer_and_Index *poi, char event[]);

void deactive_Monsters(void);

#endif