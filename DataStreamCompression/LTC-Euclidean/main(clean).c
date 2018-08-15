#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "circles_intersect.h"

#define EPSILON 100
#define TIMEUNIT_DIFFERENT 1

//#include "local-mohamand-3d.c"
#include "local-data-2d.c"

struct DATA_POINT base_data;
struct DATA_POINT coming_data;
struct CIRCLE corresponse_circle;
struct LIST_CIRCLE all_circles = {NULL, 0};



void updateBasePoint()
{
    int i;
    for(i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
    {
        base_data.data.coordinate[i] += (centre_point.coordinate[i] - base_data.data.coordinate[i])
                *(coming_data.timestamp-1 -base_data.timestamp);
    }
    base_data.timestamp = coming_data.timestamp-1;
}

void updateCorCircle()
{
    double ln = coming_data.timestamp - base_data.timestamp;
    int i;
    for(i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
    {
        corresponse_circle.centre.coordinate[i] = base_data.data.coordinate[i]
                + (coming_data.data.coordinate[i] - base_data.data.coordinate[i])/ln;
    }
    corresponse_circle.radius = EPSILON/ln;
}

void transmitData() {
    int i;
    for (printf("%u", base_data.timestamp),i = 0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        printf(",%f", base_data.data.coordinate[i]);
	printf("\n");
}

int main()
{
    int i, number=0;
    for(i=0; i< 613; i++)
    {
        coming_data = data_list[i];
        /* do initial when first and second data coming */
        if(i==0)
        {
            base_data = coming_data;
            continue;
        }

        updateCorCircle();

        if(!isThereIntersection(all_circles, corresponse_circle))
        {
            transmitData();

            // update basePoint
            updateBasePoint();

            // reset all_circle list
            reInitialList(&all_circles);

            updateCorCircle();
            addCircleIntoList(&all_circles, corresponse_circle);
            centre_point = corresponse_circle.centre;
            number+=1;
        }
        else
            addCircleIntoList(&all_circles, corresponse_circle);
    }

    transmitData();

    double ln = coming_data.timestamp - base_data.timestamp;
    for (printf("%u", coming_data.timestamp), i = 0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        printf(",%f", base_data.data.coordinate[i] + (centre_point.coordinate[i]-base_data.data.coordinate[i])*ln)
	printf("\n");

//    fclose(f_csv);
//    fclose(f_out);
}


