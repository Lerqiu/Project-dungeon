#ifndef Object_key_events
#define Object_key_events

#include <gtk/gtk.h>

#include "Objects_basic_types.h"

void characterGetKey(BattlegroundDynamic_element *object);
void characterKeyUse(BattlegroundDynamic_element *object, int key);

void Synchronization_key_vanish(int indexY, int indexX);

#endif