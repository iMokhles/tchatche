CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXEC=server.exe
CFGLIB=`pkg-config --cflags --libs glib-2.0`
all: $(EXEC)

server.exe: protocol_primitives.o protocol_instance.o function.o server.o
		$(CC) $(CFGLIB) $(LDFLAGS) -o $@ $^

function.c: function.h
protocol_instance.c: protocol_instance.h
protocol_primitives.c: protocol_primitives.h

%.o: %.c
		$(CC) -o $@ -c $< $(CFGLIB) $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)
