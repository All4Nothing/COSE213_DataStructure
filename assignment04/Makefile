CC = gcc

.c.o: 
	$(CC) -c $<

all: name_dlist

name_dlist: name.o adt_dlist.o
	$(CC) -o $@ name.o adt_dlist.o
	
clean:
	rm -f *.o
	rm -f name_dlist
