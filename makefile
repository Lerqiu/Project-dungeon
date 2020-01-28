.PHONY: clean

CC=gcc
NAME=ProjectDungeon

FILES=main.o
FILES+=Battleground_add_object_to_window.o
FILES+=Battleground_set_view.o
FILES+=Default_settings.o
FILES+=Map_loader_dynamic_elements.o
FILES+=Map_loader_prototype.o
FILES+=Map_loader_statics_elements.o
FILES+=Menu_window_functionalities.o
FILES+=Menu_window_view.o
FILES+=Object_character_events.o
FILES+=Object_gate_events.o
FILES+=Object_key_events.o
FILES+=Object_monster_events.o
FILES+=Object_princess_events.o
FILES+=Object_trap_events.o
FILES+=Objects_basic_types.o
FILES+=Objects_events_synchronization.o
FILES+=Objects_events.o
FILES+=Set_connections_betwen_players.o
FILES+=Windows_FIFO.o


$(NAME): ${FILES}
		$(CC) ${FILES} `pkg-config --libs gtk+-3.0` -lm -g -o $@ 

%.o: %.c *.h
		$(CC) $< -c `pkg-config --cflags gtk+-3.0` -lm -g -o $@ 

clean:
		rm *.o AtoB BtoA $(NAME)