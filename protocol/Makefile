CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXEC=protocol.exe

all: $(EXEC)

protocol.exe: protocol_primitives.o protocol_instance.o main.o
		$(CC) $(LDFLAGS) -o $@ $^

protocol_primitives.c: protocol_primitives.h

protocol_instance.c: protocol_instance.h

%.o: %.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)
