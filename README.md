# Cardinality Calculation over Sliding Window

[![Build Status](https://travis-ci.org/big-data-lab-team/stream-summarization.svg?branch=master)](https://travis-ci.org/big-data-lab-team/stream-summarization)

1. `DbLinkList` in `DistinctElement\DoubleLinkedList` is a simple implementation for counting distinct element in sliding windows.
It stores data in the window as a double-kinked list,
and it uses a hash table to look it up in constant time. We use
[uthash](https://troydhanson.github.io/uthash)
to implement the hash table.

2. `LRU-LC` which is a Estimation algorithm is include in  files. 
Detail of this algorithm, please read [LRU-LC: Fast Estimating Cardinality of Flows over Sliding Windows](https://www.jstage.jst.go.jp/article/transinf/E99.D/10/E99.D_2015EDL8263/_pdf)

3. `LTC` in `DataStreamCompression\LTC` is a simple implementation for Compress data by using Lightweight Temporal Compression(LTC) method.
More detail, please read [Lightweight Temporal Compression of Microclimate Datasets](https://escholarship.org/uc/item/6zg2n1rh)

4. `LTC-Manhattan` in `DataStreamCompression\LTC-Manhattan` is a method for Multi-parameters dataset. It is extention of `LTC` to compress dataset with Manhatten distance(between data).

5. Working on --- `LTC-Euclidean` in `DataStreamCompression\LTC-Euclidean` is written for compressing Multi-parameters dataset which parameters are Interrelated. 

## Installation

1. Clone the project:
``` bash
$ git clone https://github.com/big-data-lab-team/stream-summarization.git
```

2. Change directory
``` bash
$ cd stream-summarization
```

3. Clone utash
```git
git clone https://github.com/troydhanson/uthash.git
```

4. Copy `uthash.h` to the project folder
```bash
$ cp uthash/src/uthash.h ./DistinctElement/DoubleLinkedList/
```

5. Compile the project 
``` bash
$ make
```

## Execution Example

Run `cardinality`
``` bash
./cardinallity
```
Run `estimation` for running the LRU-LC estimation algorithm
``` bash
./estimation
```
Run `LTC` for running LTC data compression algorithm
```bash
./LTC-method
```
Run `LTC-Man` for running LTC-Manhattan compression algorithm by typing `./LTC-man {csv files with splliter ','}` there is a example
```bash
./LTC-man data.csv
```

