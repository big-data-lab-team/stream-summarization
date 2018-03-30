HEADERS = DbLinkList.h uthash.h

default: dbLi

dbLi.o: DbLinkList.c  $(HEADERS)
	gcc -c DbLinkList.c -o dbLi.o

main.o: main.c $(HEADERS)
	gcc -c main.c -o main.o 

dbLi: dbLi.o main.o
	gcc dbLi.o main.o -lm -o dbLi 

clean:
	-rm -f dbLi.o
	-rm -f main.o
	-rm -f dbLi
