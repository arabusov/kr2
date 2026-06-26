CFLAGS=-g -ggdb
LDFLAGS=-g -ggdb
LD=$(CC)

all: predpar lval

predpar: predpar.c
	$(LD) $(LDFLAGS) -o predpar predpar.c
lval: lval.c
	$(LD) $(LDFLAGS) -o lval lval.c

doc: notes.ps.Z

notes.ps.Z: notes.ps
	rm -f $@
	compress notes.ps
notes.ps: notes.ms
	groff -ms notes.ms > notes.ps
clean:
	rm -f notes.ps notes.ps.Z
	rm -f predpar lval
