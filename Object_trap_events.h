#ifndef Object_trap_events
#define Object_trap_events

#include <gtk/gtk.h>

#include "Objects_basic_types.h"

bool characterStepOnTrap(BattlegroundDynamic_element *object);
void Synchronization_launched_trap(int indexY, int indexX);

#endif