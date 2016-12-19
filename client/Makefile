CC=gcc
CFLAGS=-Wall
EXEC=client

all: $(EXEC)

client: client.c protocol_primitives.o protocol_instance.o
		$(CC) -o $@ $^

protocol_primitives.c: protocol_primitives.h

protocol_instance.c: protocol_instance.h

%.o: %.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf *.o
		rm -rf $(EXEC)
