CC=cc
CFLAGS=-I.
LD=cc
LDFLAGS=-i
.SUFFIXES: .o.c
OBJ=05_const.o test_cconst.o test_iconst.o tests.o

EXE=run_tests

all: $(EXE) $(OBJ) Makefile

05_const.o: 05_const.c
	$(CC) $(CFLAGS) -o 05_const.o -c 05_const.c
test_cconst.o: test_cconst.c
	$(CC) $(CFLAGS) -o test_cconst.o -c test_cconst.c
test_iconst.o: test_iconst.c
	$(CC) $(CFLAGS) -o test_iconst.o -c test_iconst.c
tests.o: tests.c
	$(CC) $(CFLAGS) -o tests.o -c tests.c

$(EXE): $(OBJ) $(HDR) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f core
