CC=g++
CFLAGS=-g -Wall

OBJ=obj
SRC=src

BIN=bin/main
OBJs=$(OBJ)/CalendarEvent.o $(OBJ)/main.o $(OBJ)/CalendarManager.o $(OBJ)/SerialCom.o
SRCs=$(SRC)/CalendarManager.cpp $(SRC)/CalendarEvent.cpp $(SRC)/main.cpp $(SRC)/SerialCom.cpp

all:bin/main

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN)

$(BIN): $(OBJs)
	$(CC) $(CFLAGS) $(OBJs) -o $@ -Llib -lchilkat

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r bin/* obj/* 

cleanW:
	del /q /s bin
	del /q /s obj