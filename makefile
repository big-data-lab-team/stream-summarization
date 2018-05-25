HEADERS = DistinctElement/DoubleLinkedList/DbLinkList.h uthash.h

default: cardinality estimation

cardinality.o: DistinctElement/DoubleLinkedList/DbLinkList.c  $(HEADERS)
	gcc -c DbLinkList.c -o cardinality.o

main.o: DistinctElement/DoubleLinkedList/main.c $(HEADERS)
	gcc -c DistinctElement/DoubleLinkedList/main.c -o main.o 

cardinality: cardinality.o main.o
	gcc cardinality.o main.o -lm -o cardinality 

estimation: DistinctElement/LRU_LC/LRU_LC.c DistinctElement/LRU_LC/LRU_LC.h
	gcc LRU_LC.c -lm -o estimation
	
ltc-method: DataStreamCompression/LTC/LTC.c DataStreamCompression/LTC/LTC.h
	gcc DataStreamCompression/LTC/LTC.c DataStreamCompression/LTC/main.c -lm -o LTC-method
	
clean:
	-rm -f cardinality.o
	-rm -f main.o
	-rm -f cardinality
	-rm -f estimation
	-rm -f LTC-method
