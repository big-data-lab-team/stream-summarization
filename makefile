HEADERS = DbLinkList.h uthash.h

default: cardinality

cardinality.o: DbLinkList.c  $(HEADERS)
	gcc -c DbLinkList.c -o cardinality.o

main.o: main.c $(HEADERS)
	gcc -c main.c -o main.o 

cardinality: cardinality.o main.o
	gcc cardinality.o main.o -lm -o cardinality 

clean:
	-rm -f cardinality.o
	-rm -f main.o
	-rm -f cardinality
