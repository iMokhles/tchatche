CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXEC=server.exe
CFGLIB=`pkg-config --cflags --libs glib-2.0`


all: $(EXEC) 

$(EXEC) : protocol_instance.o protocol_primitives.o server.o function.o 
	$(CC) -o $(EXEC) $^ $(CFGLIB) $(CFLAGS)
	
function.o: function.c function.h
	$(CC) -o $@ -c $<  $(CFGLIB) $(CFLAGS)

server.o: server.c protocol_instance.h protocol_primitives.h
	$(CC) -o $@ -c $< $(CFGLIB) $(CFLAGS)

protocol_instance.o : protocol_instance.c protocol_instance.h
	$(CC) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
	
protocol_primitives.o : protocol_primitives.c protocol_primitives.h
	$(CC) -o $@ -c $< $(LDFLAGS) $(CFLAGS)
	
clean:
	rm -rf *.o
