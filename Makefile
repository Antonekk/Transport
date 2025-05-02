CC = gcc
FLAGS = -std=gnu17 -Wall -Wextra
PROD_FLAGS = -Werror

SOURCE = ./source/main.c 
OBJ = ./source/main.o
EXEC = ./build/trasport


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

$(OBJ): $(SOURCE)
	$(CC) $(FLAGS) -c $^ -o $@


clean: 
	rm -f $(OBJ)

distclean: clean
	rm -f $(OBJ) $(EXEC)

run:
	./$(EXEC)