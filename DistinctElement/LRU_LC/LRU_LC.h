#ifndef LRU_LC_H_INCLUDED
#define LRU_LC_H_INCLUDED

struct LRU_entry {
    int forward_index;
    int backward_index;
    int time_diff;
};

struct LRU_Queue{
    int head_index;
    int tail_index;
};

// add a new LRU_entry if the status of entry is 0(inactive)
struct LRU_entry * newLRUEntry(int time_diff);

void moveEntryToQueueTail(struct LRU_entry **entry, int index, int time_diff);

void addInactiveEntryToQueueTail(struct LRU_entry **entry, int index);

int updateQueue();

void removeAndFreeEntryFromQueue();

#endif // LRU-LC_H_INCLUDED
