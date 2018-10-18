# Multi-dimensional extension of the Lightweight Temporal Compression method

[![Build Status](https://travis-ci.org/big-data-lab-team/stream-summarization.svg?branch=master)](https://travis-ci.org/big-data-lab-team/stream-summarization)

1. `LTC` is a simple implementation for Compress data by using Lightweight Temporal Compression(LTC) method.
More detail, please read [Lightweight Temporal Compression of Microclimate Datasets](https://escholarship.org/uc/item/6zg2n1rh).
You can find simple implementation in folder `LTC`

4. `Infinity-norm.c` in folder `Experiment1` is a implementation of LTC with infinity norm. We provide 4 data files in C language, each one is a array. You can change dataset and modify 'DIMENSION_WITHOUT_TIMESTAMP' macro in C file with correct value before compiling it.

5. `Euclidean-norm.c` and `Euclidean-norm.h` in folder `Experiment1` is written to realize LTC with Euclidean norm. dataset which parameters are Interrelated.

We provide 4 data files in C language, each of them is a array. You can change dataset and modify `DIMENSION_WITHOUT_TIMESTAMP` macro into correct value in C file before compiling it.

## Installation

1. Clone the project:
``` bash
$ git clone https://github.com/big-data-lab-team/stream-summarization.git
```

2. Change directory
``` bash
$ cd stream-summarization
```
3. Compile the project
``` bash
$ make
```

## Execution Example

Run original `LTC` compression algorithm
```bash
./ltc-original
```
Run `LTC` with `Infinity norm`
``` bash`
./ltc-infinity
```
Run `LTC` with `Euclidean norm`
``` bash`
./ltc-euclidean
```

## Measure Example
This capture shows how to measure Compression Ratio and Max Error, that are mentioned in [A multi-dimensional extension of the LTC method](https://github.com/big-data-lab-team/paper-multidimensional-ltc.git).

This example uses `short-bicep-curl-2d` data Set.

#### LTC with Infinity norm
Select data set `short-bicep-curl-2d.c`, add in `Infinity-norm.c`
```
#include short-bicep-curl-2d.c
```
Change `DIMENSION_WITHOUT_TIMESTAMP` macro to `2` in `Infinity-norm.c`, because we are using 2-dimensional data set.
```
#define DIMENSION_WITHOUT_TIMESTAMP 2
```
Change `EPSILON` macro to the value you want in `Infinity-norm.c`.
In this example we set `EPSILON = 100`
```
#define EPSILON 100
```
Compile the method
```
$ make ltc-infinity
```
Run LTC with Infinity norm, the output file is `compressed.csv`.
```
./ltc-infinity
```
Decode compressed data
```
Connect two adjacent points with a straight line,
and get data point from lines according their timestamp.
```
Compute Compression Ratio (CR)
```
CR = 1 - (# compressed data point) / (# total data point)
```
Compute Max Error. For each original data point (x,y) and reconstructed data point (m,n)
```
Error = Max(|x-m|,|y-n|)
Max_Error = maximum `Error` in data set.
```
#### LTC with Euclidean norm
Select data set `short-bicep-curl-2d.c`, add in `Euclidean-norm.c`
```
#include short-bicep-curl-2d.c
```
Change `DIMENSION_WITHOUT_TIMESTAMP` macro to `2` in `Euclidean-norm.h`, because we are using 2-dimensional data set.
```
#define DIMENSION_WITHOUT_TIMESTAMP 2
```
Change `EPSILON` macro to the value you want in `Euclidean-norm.c`.
In this example we set `EPSILON = 100`
```
#define EPSILON 100
```
Compile the method
```
$ make ltc-euclidean
```
Run LTC with Euclidean norm, the output file is `compressed.csv`.
```
./ltc-euclidean
```
Decode compressed data
```
Connect two adjacent points with a straight line,
and get data point from lines according their timestamp.
```
Compute Compression Ratio (CR)
```
CR = 1 - (# compressed data point) / (# total data point)
```
Compute Max Error. For each original data point (x,y) and reconstructed data point (m,n)
```
Error = sqrt(power(x-m,2) + power(y-n,2))
Max_Error = maximum `Error` between original and reconstructed data point.
```
