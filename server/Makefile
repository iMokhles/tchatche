CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXEC=server.exe
CFGLIB=`pkg-config --cflags --libs glib-2.0`

all: $(EXEC) 

$(EXEC) : server.o function.o
	$(CC) -o $(EXEC) $^ $(CFGLIB) $(CFLAGS)
	
function.o: function.c function.h
	$(CC) -o $@ -c $<  $(CFGLIB) $(CFLAGS)

server.o: server.c
	$(CC) -o $@ -c $^ $(CFGLIB) $(CFLAGS)

clean:
	rm -rf *.o
