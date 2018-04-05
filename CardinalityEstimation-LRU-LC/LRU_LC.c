#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "LRU_LC.h"

#define HEAD_LENGTH 3
#define WINDOW_LENGTH 200
#define TIME_UNIT 19989
#define SIZE_BITMAP 30

struct LRU_entry * bit_array[SIZE_BITMAP] = {NULL};
struct LRU_Queue lru_Queue = {-1,-1};
int amount_inactive = SIZE_BITMAP;

struct LRU_entry * newLRUEntry(int time_diff)
{
    struct LRU_entry * new_entry = (struct LRU_entry *)malloc(sizeof(struct LRU_entry));
    new_entry->backward_index = -1;
    new_entry->forward_index = -1;
    new_entry->time_diff = time_diff;
    return new_entry;
}

void addInactiveEntryToQueueTail(struct LRU_entry **entry, int index)
{
    bit_array[lru_Queue.tail_index]->backward_index = index;
    (*entry)->forward_index = lru_Queue.tail_index;
    lru_Queue.tail_index = index;
}


void moveEntryToQueueTail(struct LRU_entry **entry, int index, int time_diff)
{
    if(index == lru_Queue.head_index)
    {
        lru_Queue.head_index = (*entry)->backward_index;
    }
    else
    {
        struct LRU_entry *predecessor = bit_array[(*entry)->forward_index];
        predecessor ->backward_index = (*entry)->backward_index;
    }

    struct LRU_entry *successor = bit_array[(*entry)->backward_index];
    successor->forward_index = (*entry)->forward_index;
    successor->time_diff += (*entry)->time_diff;

    (*entry)->time_diff = time_diff;
    bit_array[lru_Queue.tail_index]->backward_index = index;
    (*entry)->forward_index = lru_Queue.tail_index;
    lru_Queue.tail_index = index;
}

int updateQueue()
{
    struct LRU_entry * head_entry = bit_array[lru_Queue.head_index];
    head_entry->time_diff --;
    //printf(" update function->timediff: %d,m: %d  ",head_entry->time_diff, amount_inactive);
    if(head_entry->time_diff<=0)
    {
        //printf(" timeff==0\t");
        removeAndFreeEntryFromQueue();
        amount_inactive++;
    }
    return amount_inactive;
}

void removeAndFreeEntryFromQueue()
{
    //printf(" remove function\t");
    struct LRU_entry * head_entry = bit_array[lru_Queue.head_index];
    bit_array[lru_Queue.head_index] = NULL;
    lru_Queue.head_index = head_entry->backward_index;
    bit_array[lru_Queue.head_index]->forward_index = -1;
    free(head_entry);
}
int doHash(int x, int param_a, int param_b)
{
    //printf(" p_a:%d, p_b:%d\t", param_a, param_b);
    return abs(param_a*x + param_b) % SIZE_BITMAP;
}

int main()
{
    char *path = "dump1.dat";
    FILE *pf = fopen(path, "rb");

    if(pf == NULL)
    {
        perror("file not open");
        exit(1);
    }

    //srand((unsigned)time(NULL));
    int param_a = rand();
    int param_b = rand();

    unsigned char head[HEAD_LENGTH];
    unsigned int timestamp[1];
    short quat[4];
    unsigned int last_timestamp = 0;
    int time_diff;

    int i;
    //get the head info, and check if get correct length data head, and check if the file end.
    while((3 == fread(head, sizeof(unsigned char),HEAD_LENGTH, pf)) && (!feof(pf)))
    {
        if(1==(int)head[0])
        {
            // read time stamp and 4 quatation
            fread(timestamp, 4, 1, pf);
            fread(quat, sizeof(short),4,pf);
            for(i=0;i< 4;i++)
            {
                quat[i] = (short)(round(quat[i]/100));
            }

            short key = quat[0] + quat[1] + quat[2] + quat[3];
            int index = doHash(key, param_a, param_b);
            //printf(" hash key:%hi,index:%d===",quat[0], index);

            time_diff = last_timestamp == 0? WINDOW_LENGTH+1 : (timestamp[0] - last_timestamp)/19989;
            last_timestamp = timestamp[0];
            // not entry in this index, means inactive
            if(bit_array[index] == NULL)
            {
                //printf("inactive hit \t");
                bit_array[index] = newLRUEntry(time_diff);
                // if LRU Queue is empty, add this entry in Queue
                if(amount_inactive == SIZE_BITMAP)
                {
                    lru_Queue.head_index = index;
                    lru_Queue.tail_index = index;
                }
                // if LRU Queue is not empty, add this entry in the tail of Queue
                else
                {
                    addInactiveEntryToQueueTail(&bit_array[index], index);
                }
                amount_inactive--;

                //printf(" head timediff: %d, m: %d-- ", bit_array[lru_Queue.head_index]->time_diff, amount_inactive);
            }

            // have entry in this index , means it's active
            else
            {
                //printf("active hit \t");
                if(index == lru_Queue.tail_index)
                {
                    //printf("cast 1");
                    bit_array[index]->time_diff += time_diff;
                }
                else
                {
                    //printf("cast 2-timediff: %d ", bit_array[index]->time_diff);
                    moveEntryToQueueTail(&bit_array[index], index, time_diff);
                }
            }

        }

        int m = updateQueue();
        if(m == 0) m=1;
        printf("  m: %d---Estimation: %d \n",m, (int)round(-SIZE_BITMAP*log(m/(double)SIZE_BITMAP)));
    }

    fclose(pf);
}

