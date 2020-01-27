#ifndef objects_events
#define objects_events

#include <gtk/gtk.h>
#include "Objects_basic_types.h"


bool isCharacterOnPath(BattlegroundDynamic_element *object, int X, int Y);
bool isColisionDynamic(BattlegroundDynamic_element *character_or_monster, BattlegroundDynamic_element *obj);
bool isCharacterInRangeOfAction(BattlegroundDynamic_element *character, BattlegroundDynamic_element *obj);
bool isHumanoidOnPath(BattlegroundDynamic_element *object, int X, int Y);
bool isA_object(char type[],int X, int Y);

gboolean set_view_center_object(gpointer dataPointer);

#endif