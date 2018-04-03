## Cardinality Calculation over Sliding window

This is a simply implement for counting distinct element in sliding window
Storing data which are within window in a double Linked List, and Hashing this data into a hash table in order to save the times it appears.

For hash table, we use **uthash**, where you could find it in <https://troydhanson.github.io/uthash/>

## dependencies
As we said, our code need **uthash**. If you do not have, please download in <https://troydhanson.github.io/uthash/>
or use git by 
```git
git clone https://github.com/troydhanson/uthash.git
```
Then please cp the file **uthash.h** into our code folder.

## Compilation

#GCC compiler

compiling with GCC£º
``` c
gcc DbLinkList.c main.c -o result -I. -lm
```
Geting result:
``` c
./result
```

#Makefile
We support a makefile also can help you compiling our code simplly.
make sure **uthash.h** is in the folder.

Do compile and  output result:
``` bash
$ make
```
remove file created by compiler:
``` bash
$ make clean
```

## Execution Example
todo 





