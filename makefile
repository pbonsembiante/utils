CFLAGS= -std=c11
listas: plist.o test.o 
	$(CC) $(CFLAGS) -o listas plist.o test.o -I.
