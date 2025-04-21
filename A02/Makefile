CC=cc
CFLAGS=-I.
LD=cc
LDFLAGS=
.SUFFIXES: .o.c
OBJ=keyw.o const.o utests.o

EXE=run_utests

all: $(EXE)

$(EXE): $(OBJ) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f core
