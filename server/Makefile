CC=gcc
CFLAGS=-Wall
EXEC=server
CFGLIB=`pkg-config --cflags --libs glib-2.0`


all: $(EXEC)
	
server: function.o server.o ../protocol/protocol_instance.o  ../protocol/protocol_primitives.o
	$(CC) -o $(EXEC) $^  $(CFGLIB) $(CFLAGS)

../protocol/protocol_instance.o : ../protocol/protocol_instance.c ../protocol/protocol_instance.h
	$(CC) -o $@ -c $< $(CFLAGS)

../protocol/protocol_instance.o : ../protocol/protocol_primitives.c ../protocol/protocol_primitives.h
	$(CC) -o $@ -c $< $(CFLAGS)

function.o: function.c function.h
	$(CC) -o $@ -c $< $(CFGLIB) $(CFLAGS)

server.o: server.c
	$(CC) -o $@ -c $^ $(CFGLIB) $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
