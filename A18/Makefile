CFLAGS=-g -ggdb
LDFLAGS=-g -ggdb

all: predpar

predpar: predpar.c
	$(CC) -o predpar predpar.c

doc: notes.ps.Z

notes.ps.Z: notes.ps
	rm -f $@
	compress notes.ps
notes.ps: notes.ms
	groff -ms notes.ms > notes.ps
clean:
	rm -f notes.ps notes.ps.Z
	rm -f predpar
