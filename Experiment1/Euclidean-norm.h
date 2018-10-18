
//#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(x,y) (x)>=(y)?(x):(y);
#define MIN(x,y) (x)<=(y)?(x):(y);
#define DIMENSION_WITHOUT_TIMESTAMP 2

#define OUTSIDE 0
#define INTERSECT 1
#define INSIDE -1

struct POINT {
    double coordinate[DIMENSION_WITHOUT_TIMESTAMP];
};

struct DATA_POINT {
    unsigned int timestamp;
    struct POINT data;
};

struct CIRCLE {
    struct POINT centre;
    double radius;
};

struct LIST_CIRCLE {
    struct CIRCLE * circles;
    int length;
};

void reInitialList(struct LIST_CIRCLE* list_circle)
{
    free(list_circle->circles);
    list_circle->circles = NULL;
    list_circle->length = 0;
}

void addCircleIntoList(struct LIST_CIRCLE* list_circles, struct CIRCLE new_circle)
{
    list_circles->length += 1;
    list_circles->circles = (struct CIRCLE *)realloc(list_circles->circles, list_circles->length * sizeof(struct CIRCLE));
    list_circles->circles[list_circles->length - 1] = new_circle;
}



// for maximum_x and minimum_x when circle intersect line


struct LIST_CIRCLE tmp_list_circles = {NULL,0};
struct POINT centre_point;
struct POINT tmp_centre;

int relationOf2Circles(struct CIRCLE pre_circle, struct CIRCLE new_circle)
{
    double square_dis=0;
    int i;
    for (i = 0; i < DIMENSION_WITHOUT_TIMESTAMP; i++) {
        square_dis += (pre_circle.centre.coordinate[i] - new_circle.centre.coordinate[i])
                      *(pre_circle.centre.coordinate[i] - new_circle.centre.coordinate[i]);
    }
    if(square_dis > (pre_circle.radius + new_circle.radius)*(pre_circle.radius + new_circle.radius))
        return OUTSIDE;
    else if (square_dis > (pre_circle.radius - new_circle.radius)*(pre_circle.radius- new_circle.radius))
        return INTERSECT;
    else return INSIDE;
}

// !at first, make sure that circle do not inside of every circle in the list_circle.
// !It means circle have intersection with anyone circle in the list_circle

int recursive(double left, double right, int param_index) {
    if(param_index ==0)
    {
        tmp_centre.coordinate[param_index] = (left+right)/2;
        return  1;
    }

    int h_index=0, l_index=0;
    double mid;
    double high, low, max, min, square_dis=0, square_dis_to_line=0;
    high = INFINITY;
    low = -INFINITY;
    int i,j;
    while(left < right)
    {
        mid = (left+right)/2;

        for(i=0; i<tmp_list_circles.length;i++)
        {

            square_dis_to_line = (mid-tmp_list_circles.circles[i].centre.coordinate[param_index]) * (mid-tmp_list_circles.circles[i].centre.coordinate[param_index]);
            for(j = param_index+1; j<DIMENSION_WITHOUT_TIMESTAMP; j++)
            {
                square_dis_to_line += (tmp_centre.coordinate[j] - tmp_list_circles.circles[i].centre.coordinate[j]) * (tmp_centre.coordinate[j] - tmp_list_circles.circles[i].centre.coordinate[j]);
            }

            square_dis = (tmp_list_circles.circles[i].radius * tmp_list_circles.circles[i].radius)
                         - square_dis_to_line;

            max = tmp_list_circles.circles[i].centre.coordinate[param_index-1] + sqrt(square_dis);
            min  = tmp_list_circles.circles[i].centre.coordinate[param_index-1] - sqrt(square_dis);

            if(i==0 || max < high) {high = max; h_index = i;}
            if(i==0 || min > low) {low = min; l_index = i;}
        }

        if(high >= low)
        {
            tmp_centre.coordinate[param_index] = mid;
            if(recursive(low, high, param_index-1)) {
                tmp_centre.coordinate[param_index] = mid;
                return 1;
            }
        }

        square_dis = 0;
        for(i=0; i<DIMENSION_WITHOUT_TIMESTAMP; i++)
        {
            square_dis += (tmp_list_circles.circles[h_index].centre.coordinate[i] - tmp_list_circles.circles[l_index].centre.coordinate[i])
                          *(tmp_list_circles.circles[h_index].centre.coordinate[i] - tmp_list_circles.circles[l_index].centre.coordinate[i]);
        }

        // x0 = x1 + (x2-x1)*(r1^2-r2^2+dis^2)/2dis^2
        double dis_AE = (tmp_list_circles.circles[l_index].radius*tmp_list_circles.circles[l_index].radius)
                        - (tmp_list_circles.circles[h_index].radius*tmp_list_circles.circles[h_index].radius)
                        + square_dis;

        double intersect_point = tmp_list_circles.circles[l_index].centre.coordinate[param_index]
                                 + (tmp_list_circles.circles[h_index].centre.coordinate[param_index] - tmp_list_circles.circles[l_index].centre.coordinate[param_index])
                                   * dis_AE / (2*square_dis);

        if(intersect_point < mid)
            right = mid - 1;
        else left = mid + 1;
    }
    return 0;
}

int isThereIntersection(struct LIST_CIRCLE* list_circle, struct CIRCLE new_circle) {
    reInitialList(&tmp_list_circles);
    if (list_circle->length == 0) {
        addCircleIntoList(&tmp_list_circles, new_circle);
        centre_point = new_circle.centre;
        return 1;
    }
    int param_index = DIMENSION_WITHOUT_TIMESTAMP - 1;

    double left = new_circle.centre.coordinate[param_index] - new_circle.radius;
    double right = new_circle.centre.coordinate[param_index] + new_circle.radius;

    int i;
    for (i = 0; i < list_circle->length; i++) {
        switch (relationOf2Circles(list_circle->circles[i], new_circle)) {
            case OUTSIDE:
                return 0;
            case INTERSECT:
                left = MAX(left,
                           (list_circle->circles[i].centre.coordinate[param_index] - list_circle->circles[i].radius));
                right = MIN(right,
                            (list_circle->circles[i].centre.coordinate[param_index] + list_circle->circles[i].radius));
                addCircleIntoList(&tmp_list_circles, list_circle->circles[i]);
                break;
            default:
                break;
        }
    }

    addCircleIntoList(&tmp_list_circles, new_circle);

    if (left > right)
        return 0;

    if(recursive(left, right, param_index) == 1){
        centre_point = tmp_centre;
        return 1;
    }else{
        return 0;
    }
}
