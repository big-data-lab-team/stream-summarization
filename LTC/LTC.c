#include "LTC.h"

void initialLastTransimitPoint(uint32_t timestamp, int16_t value, struct LTC_entity * ltc_entity)
{
    (*ltc_entity).last_transmit_point = (struct DATA_POINT *)malloc(sizeof(struct DATA_POINT));
    (*ltc_entity).last_transmit_point->timestamp = timestamp;
    (*ltc_entity).last_transmit_point->value = value;
}

void setNewulAndNewll(uint32_t timestamp, int16_t value, struct LTC_entity *ltc_entity, uint32_t EPSILON)
{
    if(ltc_entity->new_ul!=NULL || ltc_entity->new_ll!=NULL)
    {
        free((*ltc_entity).new_ul);
        free((*ltc_entity).new_ll);
    }
    (*ltc_entity).new_ul = (struct DATA_POINT *)malloc(sizeof(struct DATA_POINT));
    (*ltc_entity).new_ul->timestamp = timestamp;
    (*ltc_entity).new_ul->value = ((int32_t)(value + EPSILON)>32767) ? 32767: (value + EPSILON);

    (*ltc_entity).new_ll = (struct DATA_POINT *)malloc(sizeof(struct DATA_POINT));
    (*ltc_entity).new_ll->timestamp = timestamp;
    (*ltc_entity).new_ll->value = ((int32_t)(value - EPSILON)< -32768) ? -32768: (value - EPSILON);

}

void setULAndLL(struct LTC_entity *ltc_entity)
{
    if(ltc_entity->UL!=NULL || ltc_entity->LL!=NULL)
    {
        free((*ltc_entity).UL);
        free((*ltc_entity).LL);
    }

    (*ltc_entity).UL = (*ltc_entity).new_ul;
    (*ltc_entity).LL = (*ltc_entity).new_ll;

    (*ltc_entity).new_ul = NULL;
    (*ltc_entity).new_ll = NULL;
}

BOOL isTransmitData(struct LTC_entity *ltc_entity, uint32_t TIMEUNIT_DIFFERENT)
{

    double old_up_deriva = (double)(ltc_entity->UL->value - ltc_entity->last_transmit_point->value) /
                            (ltc_entity->UL->timestamp - ltc_entity->last_transmit_point->timestamp) / TIMEUNIT_DIFFERENT;

    double old_low_deriva = (double)(ltc_entity->LL->value - ltc_entity->last_transmit_point->value) /
                            (ltc_entity->LL->timestamp - ltc_entity->last_transmit_point->timestamp) / TIMEUNIT_DIFFERENT;


    double new_up_deriva = (double)(ltc_entity->new_ul->value - ltc_entity->last_transmit_point->value) /
                            (ltc_entity->new_ul->timestamp - ltc_entity->last_transmit_point->timestamp) / TIMEUNIT_DIFFERENT;

    double new_low_deriva = (double)(ltc_entity->new_ll->value - ltc_entity->last_transmit_point->value) /
                            (ltc_entity->new_ll->timestamp - ltc_entity->last_transmit_point->timestamp) / TIMEUNIT_DIFFERENT;

    if(old_up_deriva < new_low_deriva || old_low_deriva > new_up_deriva)
        return 1;
    else
    {
        if(old_up_deriva < new_up_deriva)
        {
            (*ltc_entity).new_ul->value = (ltc_entity->new_ul->timestamp - ltc_entity->UL->timestamp) / TIMEUNIT_DIFFERENT
                                        * old_up_deriva + ltc_entity->UL->value;
        }

        if(old_low_deriva > new_low_deriva)
        {
            (*ltc_entity).new_ll->value = (ltc_entity->new_ll->timestamp - ltc_entity->LL->timestamp) / TIMEUNIT_DIFFERENT
                                        * old_low_deriva + ltc_entity->LL->value;
        }
        return 0;
    }
}

void freeEntityMemory(struct LTC_entity *ltc_entity)
{
    free((*ltc_entity).last_transmit_point);
    free((*ltc_entity).UL);
    free((*ltc_entity).LL);
    free((*ltc_entity).new_ul);
    free((*ltc_entity).new_ll);

    (*ltc_entity).last_transmit_point = NULL;
    (*ltc_entity).UL = NULL;
    (*ltc_entity).LL = NULL;
    (*ltc_entity).new_ul = NULL;
    (*ltc_entity).new_ll = NULL;
}

