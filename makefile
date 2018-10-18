default:   ltc-original ltc-infinity ltc-euclidean

ltc-original: LTC/LTC.c LTC/LTC.h
	gcc LTC/LTC.c LTC/main.c -lm -o ltc-original

ltc-infinity:
	gcc Experiment1/Infinity-norm.c -o ltc-infinity

ltc-euclidean: Experiment1/Euclidean-norm.h
	gcc Experiment1/Euclidean-norm.c -lm -o ltc-euclidean

clean:
	-rm -f ltc-original
	-rm -f ltc-infinity
	-rm -f ltc-euclidean
