.PHONY: START clean

CC=gcc
NAME=ProjectDungeon

$(NAME): *.c *.h
		$(CC) *.c  `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -lm -g -o $(NAME) -fsanitize=address

clean:
		rm *.o AtoB BtoA $(NAME)