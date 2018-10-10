#include <stdio.h>

#include "ltc-eu.h"

#define EPSILON 100
#define TIMEUNIT_DIFFERENT 1

#include "local-data-2d.c"
//#include "local-data-3d.c"
//#include "local-mohamand-3d.c"
//#include "local-mohamand-2d.c"

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
    FILE * f_out = fopen("tmp.csv", "w");
    int data_index, i;
    for(data_index=0; data_index< data_list_length; data_index++)
    {
        coming_data = data_list[data_index];
        /* do initial when first and second data coming */

        if(data_index==0)
        {
            base_data = coming_data;
            continue;
        }

        updateCorCircle();

        if(!isThereIntersection(&all_circles, corresponse_circle))
        {
            //transmitData();
            for (fprintf(f_out, "%u", base_data.timestamp),i = 0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
                fprintf(f_out, ",%f", base_data.data.coordinate[i]);
            fprintf(f_out, "\n");

            // update basePoint
            updateBasePoint();

            // reset all_circle list
            reInitialList(&all_circles);

            updateCorCircle();
            addCircleIntoList(&all_circles, corresponse_circle);
            centre_point = corresponse_circle.centre;
        }
        else{
            reInitialList(&all_circles);
            all_circles.length = tmp_list_circles.length;
            all_circles.circles = (struct CIRCLE *)malloc(sizeof(struct CIRCLE) * tmp_list_circles.length);
            for (i = 0;  i< tmp_list_circles.length; i++) {
                all_circles.circles[i] = tmp_list_circles.circles[i];
            }
        }
    }

    //transmitData();
    for (fprintf(f_out, "%u", base_data.timestamp),i = 0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        fprintf(f_out, ",%f", base_data.data.coordinate[i]);
    fprintf(f_out, "\n");

    double ln = coming_data.timestamp - base_data.timestamp;
    for (fprintf(f_out, "%u", coming_data.timestamp), i = 0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        fprintf(f_out, ",%f", base_data.data.coordinate[i] + (centre_point.coordinate[i]-base_data.data.coordinate[i])*ln);
	fprintf(f_out, "\n");
}


