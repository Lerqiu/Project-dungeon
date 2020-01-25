#ifndef Object_gate_events
#define Object_gate_events

#include <gtk/gtk.h>

#include "Objects_basic_types.h"



bool humanoidColisionGate(BattlegroundDynamic_element *object, int oX, int oY);
void Synchronization_open_gate(int indexY, int indexX);

#endif