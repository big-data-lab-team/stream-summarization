HEADERS = DbLinkList.h uthash.h

default: estimation

estimation: LRU_LC.c LRU_LC.h
	gcc LRU_LC.c -I. -lm -o estimation
	
clean:
	-rm -f estimation
