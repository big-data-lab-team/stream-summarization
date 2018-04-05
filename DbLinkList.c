#include <stdio.h>
#include <stdlib.h>
#include<malloc.h>
#include <string.h>
#include "DbLinkList.h"


struct DbList * initList(){
    struct DbList *plist = (struct DbList *)malloc(sizeof(struct DbList));
    if(NULL == plist){
        printf("memory allocation \n");
        exit(EXIT_FAILURE);
    }
    plist->head = NULL;
    plist->tail = NULL;
    plist->listsize = 0;

    return plist;
}

struct DataStruct * newDataStruct(unsigned int timestamp, short quat[4]){
    struct DataStruct *data = (struct DataStruct *)malloc(sizeof(struct DataStruct));
    memcpy(data->quat,quat,4*sizeof(short));
    data->timestamp = timestamp;
    return data;
};


struct Node * newNode(struct DataStruct * data){
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (NULL == node){
        printf("memory allocation \n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->pPre = NULL;
    node->pNext = NULL;

    return node;
}

//insert a new node into double linked list
struct DataStruct * addNewNodeToList( struct DbList** dblist, struct Node *node){

    if( (*dblist)->listsize == 0){
        printf("???");
        (*dblist)->head = node;
        (*dblist)->tail = node;

    }
    else{
        //printf("????%d????", node->data->quat[3]);
        node->pNext = (*dblist)->head;
        (*dblist)->head->pPre = node;
        (*dblist)->head = node;
    }

    (*dblist)->listsize++;
    return node->data;
}

// remove tail element and free the memory
struct DataStruct * removeAndFreeTailOfList(struct DbList** dblist){
    struct DataStruct *data = (*dblist)->tail->data;
    struct Node *preNode = (*dblist)->tail->pPre;
    preNode->pNext = NULL;

    free((*dblist)->tail);
    (*dblist)->tail = preNode;
    (*dblist)->listsize--;

    return data;
}
