#ifndef DBLINKLIST_H_INCLUDED
#define DBLINKLIST_H_INCLUDED


struct DataStruct
{
    unsigned int timestamp;
    short quat[4];
};


struct Node
{
    struct DataStruct *data;
    struct Node *pNext;
    struct Node *pPre;
};

struct DbList
{
    struct Node *head, *tail;
    int listsize;
};

// create empty DbList
struct DbList * initList();

struct DataStruct * newDataStruct(unsigned int timestamp, short quat[4]);

//create new node of DbList with struct DataStruct
struct Node * newNode(struct DataStruct *data);

//insert a new node into double linked list
struct DataStruct * addNewNodeToList( struct DbList** dblist, struct Node *node);

// remove tail element and free the memory
struct DataStruct * removeAndFreeTailOfList(struct DbList** dblist);


#endif // DBLINKLIST_H_INCLUDED
