#include <stdio.h> // printf
#include <stdlib.h> //malloc
#include <math.h> // sqrt()

#define DIMENSION_WITHOUT_TIMESTAMP 3
#define EPSILON 100
#define TIME_UNIT_DIFFERENCE 1

#define MAX(x,y) (x)>=(y)?(x):(y);
#define MIN(x,y) (x)<=(y)?(x):(y);

struct DATA_POINT {
    int timestamp;
    double params[DIMENSION_WITHOUT_TIMESTAMP];
};

struct BOUND_BOX {
    /*
     * [n][0] = upper bound
     * [n][1] = lower bound
     */
    double bounds[DIMENSION_WITHOUT_TIMESTAMP][2];
};

//#include "local-data-2d.c"
#include "local-data-3d.c"
//#include "local-mohamand-2d.c"
//#include "local-mohamand-3d.c"

void initialBoundBox(struct BOUND_BOX *_bound_box, const struct DATA_POINT *_coming_data)
{
    int param_index;
    for(param_index=0; param_index<DIMENSION_WITHOUT_TIMESTAMP; param_index++)
    {
        _bound_box->bounds[param_index][0] = _coming_data->params[param_index] + EPSILON;
        _bound_box->bounds[param_index][1] = _coming_data->params[param_index] - EPSILON;
    }
}

void updateBoundBox(struct BOUND_BOX *_bound_box, const struct DATA_POINT *_coming_data, const struct DATA_POINT *_base_data)
{
    double time_differ = (_coming_data->timestamp - _base_data->timestamp)/(double)TIME_UNIT_DIFFERENCE;

    //changed_param = (coming_data.ax-base_data.ax)/time_differ + base_data.ax;
    // bound_box_max = MIN(changed_param + EPSILON/time_differ, bound_box_max);
    int param_index;
    for(param_index=0; param_index<DIMENSION_WITHOUT_TIMESTAMP; param_index++)
    {
        _bound_box->bounds[param_index][0] = MIN(_bound_box->bounds[param_index][0],
                                                (_base_data->params[param_index] + (_coming_data->params[param_index] - _base_data->params[param_index])/time_differ)
                                                + EPSILON/time_differ);
        _bound_box->bounds[param_index][1] = MAX(_bound_box->bounds[param_index][1],
                                                (_base_data->params[param_index] + (_coming_data->params[param_index] - _base_data->params[param_index])/time_differ)
                                                - EPSILON/time_differ);
    }
}

void updateBasePoint(const struct BOUND_BOX *_bound_box, const struct DATA_POINT *_coming_data, struct DATA_POINT *_base_data)
{
    double time_differ = (_coming_data->timestamp - TIME_UNIT_DIFFERENCE - _base_data->timestamp)/(double)TIME_UNIT_DIFFERENCE;

    _base_data->timestamp = _coming_data->timestamp - TIME_UNIT_DIFFERENCE;

    // new_x = ((bound_box.max_x + bound_box.min_x)/2 - base_data.ax)*time_differ + base_data.ax;
    int param_index;
    for(param_index=0; param_index<DIMENSION_WITHOUT_TIMESTAMP; param_index++)
    {
        _base_data->params[param_index] += ((_bound_box->bounds[param_index][0] + _bound_box->bounds[param_index][1])/2
                - _base_data->params[param_index]) * time_differ;
    }
}

double dabs(double x){
    if(x < 0)
        x = -x;
    return x;
}

int isTransmitData(const struct BOUND_BOX *_bound_box, const struct DATA_POINT *_coming_data, const struct DATA_POINT *_base_data)
{
    double time_differ = _coming_data->timestamp - _base_data->timestamp;

    int param_index;
    for(param_index=0; param_index<DIMENSION_WITHOUT_TIMESTAMP; param_index++)
    {
        //fabs(changed_x - (bound_box.max_x + bound_box.min_x)/2) > (bound_box.max_x-bound_box.min_x)/2 + EPSILON/ln_base
        if(dabs(_base_data->params[param_index] + (_coming_data->params[param_index] - _base_data->params[param_index])/time_differ -
                        (_bound_box->bounds[param_index][0] + _bound_box->bounds[param_index][1])/2)
                        >  (_bound_box->bounds[param_index][0] - _bound_box->bounds[param_index][1])/2 + EPSILON/time_differ)
            return 1;
    }
    return 0;
}


int main()
{
    FILE* f_out = fopen("tmp.csv", "w");
    struct DATA_POINT *coming_data = (struct DATA_POINT *)malloc(sizeof(struct DATA_POINT));
    struct DATA_POINT *base_data = NULL;
    struct BOUND_BOX *bound_box = NULL;

	int data_index, i;
    for(data_index=0; data_index< data_list_length; data_index++)
    {
        *coming_data = data_list[data_index];

        /* do initial when first and second data coming */
        if(base_data==NULL)
        {
            base_data = (struct DATA_POINT *)malloc(sizeof(struct DATA_POINT));
            *base_data = *coming_data;
            continue;
        }
        else if(bound_box==NULL)
        {
            bound_box = (struct BOUND_BOX *)malloc(sizeof(struct BOUND_BOX));
            initialBoundBox(bound_box, coming_data);
            continue;
        }

        if(isTransmitData(bound_box, coming_data, base_data))
        {
            for(fprintf(f_out, "%u", base_data->timestamp), i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
                fprintf(f_out, ",%f", base_data->params[i]);
            fprintf(f_out, "\n");
            
            updateBasePoint(bound_box, coming_data, base_data);
            initialBoundBox(bound_box, coming_data);
        }

        updateBoundBox(bound_box, coming_data, base_data);
    }
    
    for(fprintf(f_out, "%u", base_data->timestamp), i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        fprintf(f_out, ",%f", base_data->params[i]);
    fprintf(f_out, "\n");
    
    for(fprintf(f_out, "%u", coming_data->timestamp), i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        //fprintf(f_out, ",%f", (bound_box->bounds[i][0] + bound_box->bounds[i][1])/2);
        fprintf(f_out, ",%f", base_data->params[i] + (coming_data->timestamp - base_data->timestamp)*((bound_box->bounds[i][0] + bound_box->bounds[i][1])/2 - base_data->params[i]));
    fprintf(f_out, "\n");

    free(coming_data);
    free(base_data);
    free(bound_box);
}
