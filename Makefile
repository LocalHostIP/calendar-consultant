CC=g++
CFLAGS=-g -Wall

OBJ=obj
SRC=src

BIN=bin/main
OBJs=$(OBJ)/main.o $(OBJ)/CalendarManager.o 
SRCs=$(SRC)/main.cpp $(SRC)/CalendarManager.cpp 

all:bin/main

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN)

$(BIN): $(OBJs)
	$(CC) $(CFLAGS) $(OBJs) -o $@ -L lib -lchilkat

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r bin/* obj/* 