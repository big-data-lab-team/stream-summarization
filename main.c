#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DbLinkList.h"
#include "uthash.h"
#define HEAD_LENGTH 3
#define WINDOW_LENGTH 50

struct hashTable_struct {
    short quat[4];                    /* key */
    int count;
    UT_hash_handle hh;         /* makes this structure hashable */
};
struct hashTable_struct *hash_data = NULL;

void addDataToHashTable(struct DataStruct* outputData);
void updataDataInHashTable(struct DataStruct* outputData);
unsigned int  distinctElementCount();
void printData();


int main()
{
    char *path = "dump1.dat";
    FILE *pf = fopen(path, "rb");
    struct DataStruct *data;
    struct DataStruct *outputData;
    struct DbList *dbllist = initList();
    struct Node *node;

    if(pf == NULL){
        perror("file not open");
        exit(1);
    }
    unsigned char head[HEAD_LENGTH];
    unsigned int timestamp[1];
    short quat[4];
    int i;
    //get the head info, and check if get correct length data head, and check if the file end.

    while((3 == fread(head, sizeof(unsigned char),HEAD_LENGTH, pf)) && (!feof(pf))){
        if(1==(int)head[0]){
            // read time stamp and 4 quatation
            fread(timestamp, 4, 1, pf);
            fread(quat, 2,4,pf);
            for(i=0;i<sizeof(quat);i++){
                quat[i] = (short)(round(quat[i]/1000));
                //printf("...........%hi.......\n", quat[i]);
            }

            // create new Data struct from time stamp and quaternion
            data = newDataStruct((unsigned int)timestamp, quat);
            // assign new space and create new node by using data
            node = newNode(data);
            // add this new node into DbLinkedList
            outputData = addNewNodeToList(&dbllist, node);
            // add the data of new node in to hash table, it could return cardinality count.
            addDataToHashTable(outputData);
            // check if the list(sliding window) is filled
            if(dbllist->listsize > WINDOW_LENGTH){
                // if the list(sliding window) is filled, remove the oldest element(data Node)
                outputData = removeAndFreeTailOfList(&dbllist);
                updataDataInHashTable(outputData);
            }
            // check this data in structure
            distinctElementCount();
        }
    }

    fclose(pf);
    printData();
}

void addDataToHashTable(struct DataStruct * outputData){
    struct hashTable_struct *s;
    //int id = outputData->quat;
    HASH_FIND_INT(hash_data, &(outputData->quat), s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct hashTable_struct*)malloc(sizeof(struct hashTable_struct));
      //s->id = outputData->quat;
      memcpy(s->quat, outputData->quat, 4*sizeof(short));
      s->count = 0;
      HASH_ADD_INT( hash_data, quat, s );  /* id: name of key field */
    }
    s->count++;
}

void updataDataInHashTable(struct DataStruct * outputData){
    struct hashTable_struct *s;
    HASH_FIND_INT(hash_data, &(outputData->quat), s);
    if (s==NULL) {
        printf("wrong in remove element from hashtable (updateDataInHashTable() )");
        return;
    }
    s->count--;
    if(s->count <= 0){
        // delete element from hash table
        HASH_DEL(hash_data, s);
        // release the RAM space
        free(s);
    }
}

unsigned int  distinctElementCount(){
    unsigned int count;
    count = HASH_COUNT(hash_data);
    printf("there are %u hash_data\n", count);
    return count;
}

void printData(){
    struct hashTable_struct *s;
    int c = 1;
    for(s=hash_data; s!=NULL; s=s->hh.next){
        printf("%d--data id : %hi, count: %d --\n", c++,s->quat[3], s->count);
    }
}

