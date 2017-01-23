IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -std=c11

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = plist.h pnode.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = plist.o test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

plist: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
<<<<<<< HEAD
	rm -f $(ODIR)/*.o
	rmdir $(ODIR)
	
install: 
	mkdir obj
=======
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
>>>>>>> 7aa2cdae3cc53fb7bce9dfb57de19afca48011b5
