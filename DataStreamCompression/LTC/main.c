#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "LTC.h"
#define MAX_lINE_BYPE_SIZE 500


#define IS_CSV_INCLUDE_TITLE 1
#define NUMBER_OF_FEATURES 3

#define EPSILON 50 // error margin
#define TIMEUNIT_DIFFERENT 1

struct DATA_STRUCT{
    uint32_t timestamp;
    int16_t parameter[NUMBER_OF_FEATURES];
};

void initialLTC_entities(struct LTC_entity * ltc_entity)
{
        ltc_entity->last_transmit_point = NULL;
        ltc_entity->UL = NULL;
        ltc_entity->LL = NULL;
        ltc_entity->new_ul = NULL;
        ltc_entity->new_ll = NULL;
}

int processNewData(uint32_t timestamp, int16_t parameter, struct LTC_entity * ltc_entity)
{
        if(ltc_entity->last_transmit_point == NULL)
        {
            initialLastTransimitPoint(timestamp, parameter, ltc_entity);
            return 1;
        }
        setNewulAndNewll(timestamp, parameter, ltc_entity, EPSILON);
        if(ltc_entity->UL==NULL || ltc_entity->LL==NULL)
        {
            setULAndLL(ltc_entity);
            return 1;
        }
        return 0;
}


int main()
{
    char *fpath = "data.csv";
    FILE * f_csv = fopen(fpath, "r");
    if(f_csv == NULL)
    {
        printf("Open file %s failed", fpath);
        exit(0);
    }

    FILE * foutput = fopen("result.csv","w");
    fprintf(foutput,"Timestamp");

    struct DATA_STRUCT coming_data; // the struct which save the data
    char strLine[MAX_lINE_BYPE_SIZE];   // used for reading line in CSV
    char *tmp_segment; // save the segment which is splitted from strLine

    struct LTC_entity ltc_entities[NUMBER_OF_FEATURES];
    int time_counter = 0; // time counter to simulate time stamp;
    int j; // used for index parameter
    int ifContinue; // used for check if need to continue the loop
    int isTransmit; // used for check if need to transmit data after processing coming_data

    /*    initial LTC entities    */
    for(j=0; j<NUMBER_OF_FEATURES; j++)
    {
        initialLTC_entities(&ltc_entities[j]);
    }

    if(IS_CSV_INCLUDE_TITLE)
        fgets(strLine, MAX_lINE_BYPE_SIZE, f_csv);

    /*  reading CSV file line by line  */
    while(fgets(strLine, MAX_lINE_BYPE_SIZE, f_csv))
    {
        time_counter ++;

        /*   catch data and save it into coming_data struct    */
        tmp_segment = strtok(strLine, ",");
        if(tmp_segment != NULL)
        {
            coming_data.timestamp = time_counter;
        }
        for(j=0,tmp_segment=strtok(NULL, ","); tmp_segment != NULL && j<NUMBER_OF_FEATURES; j++, tmp_segment=strtok(NULL, ","))
        {
            coming_data.parameter[j] = (int16_t)atoi(tmp_segment);
        }

        /*  first step, set new_ul and new_ll with coming_data  */
        ifContinue = 0; // if is the first data point (point z) and second data, need continue this loop
        for(j=0; j<NUMBER_OF_FEATURES; j++)
        {
            ifContinue = processNewData(coming_data.timestamp, coming_data.parameter[j], &ltc_entities[j]) || ifContinue;
        }
        if(ifContinue) continue;

        /* second step, check if need transmission */
        isTransmit = 0;
        for(j=0; j<NUMBER_OF_FEATURES; j++)
        {
            isTransmit = isTransmitData(&ltc_entities[j], TIMEUNIT_DIFFERENT) || isTransmit;
        }
        if(isTransmit)
        {
            //todo print out data
            fprintf(foutput,"\n%u",ltc_entities[0].last_transmit_point->timestamp);
            for(j=0; j<NUMBER_OF_FEATURES; j++)
            {
                fprintf(foutput,",%hi",ltc_entities[j].last_transmit_point->value);
            }

            int16_t tmp_param;
            for(j=0; j<NUMBER_OF_FEATURES; j++)
            {
                tmp_param = (ltc_entities[j].UL->value + ltc_entities[j].LL->value) / 2;
                freeEntityMemory(&ltc_entities[j]);
                initialLastTransimitPoint(coming_data.timestamp-1, tmp_param, &ltc_entities[j]);
                processNewData(coming_data.timestamp, coming_data.parameter[j], &ltc_entities[j]);
            }
        }
        else
        {
            for(j=0; j<NUMBER_OF_FEATURES; j++)
            {
                setULAndLL(&ltc_entities[j]);
            }
        }
    }

    fprintf(foutput,"\n%u",time_counter);
    for(j=0; j<NUMBER_OF_FEATURES; j++)
    {
        fprintf(foutput,",%hi",(ltc_entities[j].UL->value + ltc_entities[j].LL->value) / 2);
    }

    fclose(foutput);
    fclose(f_csv);
}
