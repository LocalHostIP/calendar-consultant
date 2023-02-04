CC=g++
CFLAGS=-g -Wall

BIN=bin/main
OBJ=obj/main.o
SRC=src/main.cpp

all:bin/main

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) obj/main.o -o $@ -L lib -lchilkat

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r bin/* obj/* 