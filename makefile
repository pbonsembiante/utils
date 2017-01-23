CFLAGS= -std=c11
listas: plist.o test.o 
	$(CC) -o listas plist.o test.o -I. $(CFLAGS)
