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
``` console
$ git clone https://github.com/big-data-lab-team/stream-summarization.git
```

2. Change directory
``` console
$ cd stream-summarization
```
3. Compile the project
``` console
$ make
```

## Execution Example

Run original `LTC` compression algorithm
```console
./ltc-original
```
Run `LTC` with `Infinity norm`
``` console
./ltc-infinity
```
Run `LTC` with `Euclidean norm`
``` console
./ltc-euclidean
```

## Measure Example
This capture shows how to measure Compression Ratio and Max Error, that are mentioned in [A multi-dimensional extension of the LTC method](https://github.com/big-data-lab-team/paper-multidimensional-ltc.git).

This example uses `short-bicep-curl-2d` data Set.

Note that the data points in this data set are sampling from accelerometer
sensor, where the accelerometer's `range` is configured to be ±2g. The `error
tolerance` equals `EPSILON * range / 2^15` in g.

#### LTC with Infinity norm
Select data set `short-bicep-curl-2d.c`, add in `Infinity-norm.c`
```c
#include short-bicep-curl-2d.c
```
Change `DIMENSION_WITHOUT_TIMESTAMP` macro to `2` in `Infinity-norm.c`, because we are using 2-dimensional data set.
```c
#define DIMENSION_WITHOUT_TIMESTAMP 2
```
Change `EPSILON` macro to the value you want in `Infinity-norm.c`.
In this example we set `EPSILON = 800`, corresponding to 48.8 mg.
```c
#define EPSILON 800
```
Compile the method
```
$ make ltc-infinity
```
Run LTC with Infinity norm, the output file is `compressed-infinity.csv`.
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
```c
#include short-bicep-curl-2d.c
```
Change `DIMENSION_WITHOUT_TIMESTAMP` macro to `2` in `Euclidean-norm.h`, because we are using 2-dimensional data set.
```c
#define DIMENSION_WITHOUT_TIMESTAMP 2
```
Change `EPSILON` macro to the value you want in `Euclidean-norm.c`.
In this example we set `EPSILON = 800`, corresponding to 48.8 mg.
```c
#define EPSILON 800
```
Compile the method
```
$ make ltc-euclidean
```
Run LTC with Euclidean norm, the output file is `compressed-euclidean.csv`.
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
## Measure processing time
Processing time is measured by gettimeofday() from the GNU C Library. You can
just remove comments from `Infinity-norm.c` or `Euclidean-norm.c`.

Note that please remove the code about writing, such as `fprintf`, in order to
get accurate results.


### Infinity norm
In `Infinity-norm.c`, remove the comment in line 104
```c
gettimeofday(&tv1, NULL);
```
and the comments from line 137 to 140
```c
gettimeofday(&tv2, NULL);
printf ("Total time = %f seconds\n",
      (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
      (double) (tv2.tv_sec - tv1.tv_sec));
```

### Euclidean norm

In `Euclidean-norm.c`, remove the comment in line 47
```c
gettimeofday(&tv1, NULL);
```
and the comments from line 87 to 90
```c
gettimeofday(&tv2, NULL);
printf ("Total time = %f seconds\n",
      (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
      (double) (tv2.tv_sec - tv1.tv_sec));
```
## Measure memory consumption
We measured memory consumption using [Valgrind’s massif
tool](http://valgrind.org/docs/manual/ms-manual.html)

0. At first, install `valgrind`
1. Compile programs, for example
```
$ make ltc-infinity
```
2. To gather profiling information by using `Massif`
```bash
valgrind --tool=massif ./ltc-infinity
```
3. Runing `ms_print` or `massif-visualizer` to see the information, for example
```bash
$ ms_print massif.out.<pid>
```

```bash
$ massif-visualizer
```
