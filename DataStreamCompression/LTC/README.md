# Lightweight Temporal Compression

[![Build Status](https://travis-ci.org/big-data-lab-team/stream-summarization.svg?branch=master)](https://travis-ci.org/big-data-lab-team/stream-summarization)

This is a simple implementation for Compress data by using Lightweight Temporal Compression(LTC) method.

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

