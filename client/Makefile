CC=gcc
CFLAGS=-Wall
EXEC=tmp

all: $(EXEC)

tmp: ihm.c
				$(CC) -o $@ $^ -lpanel -lncurses

clean:
				rm -rf *.o

mrproper: clean
				rm -rf *.o
				rm -rf $(EXEC)
