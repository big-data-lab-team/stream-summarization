#ifndef LTC_H_INCLUDED
#define LTC_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define BOOL int


struct DATA_POINT {
    uint32_t timestamp;
    int16_t value;
};

struct LTC_entity {
    struct DATA_POINT * last_transmit_point;
    struct DATA_POINT * UL;
    struct DATA_POINT * LL;
    struct DATA_POINT * new_ul;
    struct DATA_POINT * new_ll;
};

//void initialLTCEntry(struct LTC_entity *ltc_entity);

void initialLastTransimitPoint(uint32_t timestamp, int16_t value, struct LTC_entity * ltc_entity);

void setNewulAndNewll(uint32_t timestamp, int16_t value, struct LTC_entity *ltc_entity, uint32_t EPSILON);

void setULAndLL(struct LTC_entity *ltc_entity);

BOOL isTransmitData(struct LTC_entity *ltc_entity, uint32_t TIMEUNIT_DIFFERENT);

void freeEntityMemory(struct LTC_entity *ltc_entity);


#endif // LTC_H_INCLUDED
