.PHONY: START clean

CC=gcc
NAME=ProjectDungeon

FILES=main.o
FILES+=battleground.o
FILES+=battleground.o
FILES+=battleground.o
FILES+=battleground.o
FILES+=battleground.o
$(NAME): ${FILES}
		$(CC) ${FILES} `pkg-config --libs gtk+-3.0` -lm -g -o $@ -fsanitize=address

%.o: %.c *.h
		$(CC) $< -c `pkg-config --cflags gtk+-3.0` -lm -g -o $@ -fsanitize=address

clean:
		rm *.o AtoB BtoA