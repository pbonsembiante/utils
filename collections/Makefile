IDIR = ./include
CFLAGS= -I$(IDIR) -std=c11 -Wall -Wextra

ODIR= obj
LDIR = ../lib

LIBS=-lm

_DEPS = plist.h pnode.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = plist.o test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

plist: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean install

clean:
	rm -f $(ODIR)/*.o
	rmdir $(ODIR)
	
install: 
	mkdir obj