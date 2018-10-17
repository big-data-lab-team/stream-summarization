HEADERS = DistinctElement/DoubleLinkedList/DbLinkList.h DistinctElement/DoubleLinkedList/uthash.h

default: cardinality estimation ltc-method ltc-man ltc-eu Experiment-1 Experiment-2

cardinality.o: DistinctElement/DoubleLinkedList/DbLinkList.c  $(HEADERS)
	gcc -c DistinctElement/DoubleLinkedList/DbLinkList.c -o cardinality.o

main.o: DistinctElement/DoubleLinkedList/main.c $(HEADERS)
	gcc -c DistinctElement/DoubleLinkedList/main.c -o main.o

cardinality: cardinality.o main.o
	gcc cardinality.o main.o -lm -o cardinality

estimation: DistinctElement/LRU_LC/LRU_LC.c DistinctElement/LRU_LC/LRU_LC.h
	gcc DistinctElement/LRU_LC/LRU_LC.c -lm -o estimation

ltc-method: DataStreamCompression/LTC/LTC.c DataStreamCompression/LTC/LTC.h
	gcc DataStreamCompression/LTC/LTC.c DataStreamCompression/LTC/main.c -lm -o LTC-method

ltc-man:
	gcc DataStreamCompression/LTC-Manhattan/ltc-man.c -o LTC-man

ltc-eu:
	gcc DataStreamCompression/LTC-Manhattan/ltc-man.c -lm -o LTC-eu

Experiment-1:
	gcc DataStreamCompression/Experiment1/Infinity-norm.c -lm -o Ex1-infinity
	gcc DataStreamCompression/Experiment1/Euclidean-norm.c -lm -o Ex1-euclidean

Experiment-2:
	gcc DataStreamCompression/Experiment2/main.cpp -lm -o Ex2

clean:
	-rm -f cardinality.o
	-rm -f main.o
	-rm -f cardinality
	-rm -f estimation
	-rm -f LTC-method
	-rm -f LTC-man
	-rm -f LTC-eu
	-rm -f Ex1-infinity
	-rm -f Ex1-euclidean
	-rm -f Ex2
