CC=gcc
CFLAGS=-std=gnu17 -ggdb -Wall -Werror -pedantic
LDFLAGS=-lm -lSDL2

SRC=$(wildcard src/*.c)
OBJS=$(notdir $(SRC:.c=.o))

tetris: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	-rm *.o tetris

