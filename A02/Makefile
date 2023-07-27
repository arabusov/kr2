CC=gcc
CFLAGS=-ansi -pedantic -Wall
LD=gcc

SRC=$(shell ls *.c)
HRD=$(shell ls *.h)
OBJ=$(patsubst %.c, %.o, $(SRC))

EXE=test

all: $(EXE) $(OBJ) $(HDR) Makefile

$(EXE): $(OBJ) $(HDR) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJ)
	rm -f $(EXE)
