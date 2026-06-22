CFLAGS=-ansi -pedantic -Wall -Wextra -Werror -Wfatal-errors --coverage
CFLAGS+=-I.
CFLAGS+=-g -ggdb
LD=$(CC)
LDFLAGS=-g -ggdb --coverage
.SUFFIXES: .o.c
SCANO=ident.o table.o keyw.o const.o tok.o scan.o
OBJ=$(SCANO) parse.o
TESTO=$(SCANO) utests.o

TESTS=run_utests

EXE=parse

.PHONY: all
all: $(TESTS) $(EXE)

.PHONY: test
test: $(TESTS)
	@./run_utests

.PHONY: cov
cov: test
	mkdir -p coverage
	@rm -rf utests.gc*
	@gcovr --txt
	@gcovr --html-self-contained --html-details -o coverage/A02.html

$(EXE): $(OBJ) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)
$(TESTS): $(TESTO) Makefile
	$(LD) $(LDFLAGS) -o $(TESTS) $(TESTO)

.PHONY: clean
clean:
	rm -f $(OBJ)
	rm -f utests.o
	rm -f $(EXE)
	rm -f $(TESTS)
	rm -f core
	rm -f *.gc*
	rm -rf coverage
