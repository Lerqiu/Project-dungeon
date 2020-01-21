.PHONY: START clean

CC=gcc
NAME=ProjectDungeon

$(NAME): 
		$(CC) *.c `pkg-config --cflags gtk+-3.0` ` pkg-config --libs gtk+-3.0` -lm -g -o $(NAME)

clean:
		rm *.o