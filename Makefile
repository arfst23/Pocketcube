################################################################################

CC		= gcc
CFLAGS		= -mcmodel=large -O3 -march=native -funroll-loops
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

all: solver

################################################################################

pocket.o: pocket.h

mksol.o: pocket.h

mksol: pocket.o mksol.o
	$(REASON)
	$(LINK.c) -o $@ $^ $(LDLIBS)

solver.h: mksol
	$(REASON)
	mksol > solver.h

solver.o: pocket.h solver.h

solver: pocket.o solver.o
	$(REASON)
	$(LINK.c) -o $@ $^ $(LDLIBS)

mkpoc.o: pocket.h

mkpoc: pocket.o mkpoc.o
	$(REASON)
	$(LINK.c) -o $@ $^ $(LDLIBS)

################################################################################

tags:
	$(TAGS) $(TAGSFLAGS) -o $@ *.h *.cc

clean:
	$(RM) *.o mksol solver.h

distclean: clean
	$(RM) core err tags *~ solver

################################################################################
