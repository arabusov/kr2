CC ?= gcc
CFLAGS=-ansi -pedantic -Wall -Wextra -Werror -Wfatal-errors
CFLAGS+=-I.
CFLAGS+=-g -ggdb
LD=$(CC)
LDFLAGS=-g -ggdb
.SUFFIXES: .o.c
OBJ=ident.o table.o keyw.o const.o scan.o
TESTO=keyw.o const.o utests.o

TESTS=run_utests

EXE=scan

all: $(TESTS) $(EXE)

$(EXE): $(OBJ) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)
$(TESTS): $(TESTO) Makefile
	$(LD) $(LDFLAGS) -o $(TESTS) $(TESTO)

clean:
	rm -f $(OBJ)
	rm -f $(TESTO)
	rm -f $(EXE)
	rm -f $(TESTS)
	rm -f core
