NOTE=note
MSNOTE=$(NOTE).ms
PSNOTE=$(NOTE).ps
CNOTE=$(PSNOTE).Z
CFLAGS=-g -ggdb
LDFLAGS=-g -ggdb
LD=$(CC)

all: predpar lval

predpar: predpar.c
	$(LD) $(LDFLAGS) -o predpar predpar.c
lval: lval.c
	$(LD) $(LDFLAGS) -o lval lval.c

doc: $(CNOTE)

$(CNOTE): $(PSNOTE)
	rm -f $@
	compress $(PSNOTE)

$(PSNOTE): $(MSNOTE)
	groff -ms $(MSNOTE) > $(PSNOTE)

preview: $(CNOTE)
	nohup gv -scale=1.414 $(CNOTE) &
	rm -f nohup.out

clean:
	rm -f $(PSNOTE) $(CNOTE)
	rm -f predpar lval
	rm -f nohup.out
