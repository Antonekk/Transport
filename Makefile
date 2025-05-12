CC = gcc
CFLAGS = -std=gnu17 -Wall -Wextra


EXEC = transport
SRC = transport.c helpers.c wrappers.c
OBJ = $(SRC:.c=.o)


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

wrappers.o: wrappers.c wrappers.h
	$(CC) $(CFLAGS) -c wrappers.c -o wrappers.o

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c -o helpers.o

transport.o: transport.c 
	$(CC) $(CFLAGS) -c transport.c -o transport.o


clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(OBJ) $(EXEC)



test: 
	./transport 127.0.0.1 2115 myfile 9013041
