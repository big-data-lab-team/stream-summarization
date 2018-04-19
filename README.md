# Cardinality Calculation over Sliding Window

[![Build Status](https://travis-ci.org/big-data-lab-team/stream-summarization.svg?branch=master)](https://travis-ci.org/big-data-lab-team/stream-summarization)

This is a simple implementation for counting distinct element in sliding windows.
It stores data in the window as a double-kinked list,
and it uses a hash table to look it up in constant time. We use
[uthash](https://troydhanson.github.io/uthash)
to implement the hash table.

Additionally, LRU-LC which is a Estimation algorithm is include in `LRU-LC.c` and `LRU-LC.h` files. 
Detail of this algorithm, please read [LRU-LC: Fast Estimating Cardinality of Flows over Sliding Windows](https://www.jstage.jst.go.jp/article/transinf/E99.D/10/E99.D_2015EDL8263/_pdf)


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
$ cp uthash/src/uthash.h .
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
Run `estimation` for run the LRU-LC estimation algorithm
``` bash
./estimation
```

