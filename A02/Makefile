CC=cc
CFLAGS=-I.
LD=cc
LDFLAGS=
.SUFFIXES: .o.c
OBJ=keyw.o const.o test_cconst.o test_iconst.o test_keyw.o tests.o

EXE=run_tests

all: $(EXE)

$(EXE): $(OBJ) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f core
