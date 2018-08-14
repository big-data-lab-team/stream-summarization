//
// Created by boli on 8/3/18.
//

#ifndef LTC_EU_CIRCLES_INTERSECT_H
#define LTC_EU_CIRCLES_INTERSECT_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define DIMENSION_WITHOUT_TIMESTAMP 2

#define OUTSIDE 0
#define INTERSECT 1

struct POINT {
    double x,y,z;
};

struct DATA_POINT {
    uint32_t timestamp;
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

void addCircleIntoList(struct LIST_CIRCLE* list_circles, struct CIRCLE new_circle)
{
    list_circles->length += 1;
    list_circles->circles = (struct CIRCLE *)realloc(list_circles->circles, list_circles->length * sizeof(struct CIRCLE));
    list_circles->circles[list_circles->length - 1] = new_circle;
}

double left, right;

// for maximum_x and minimum_x when circle intersect line


struct LIST_CIRCLE tmp_list_circles = {NULL,0};
struct POINT centre_point;


int recursive1(double left, double right);

int recursive2(double left, double right, double x);

int relationOf2Circles(struct CIRCLE pre_circle, struct CIRCLE new_circle)
{
    double sqar_dis = (pre_circle.centre.x - new_circle.centre.x)*(pre_circle.centre.x - new_circle.centre.x)
                      + (pre_circle.centre.y - new_circle.centre.y)*(pre_circle.centre.y - new_circle.centre.y)
                      + (pre_circle.centre.z - new_circle.centre.z)*(pre_circle.centre.z - new_circle.centre.z);
    if(sqar_dis > (pre_circle.radius + new_circle.radius)*(pre_circle.radius + new_circle.radius))
        return OUTSIDE;
    else if (sqar_dis > (pre_circle.radius - new_circle.radius)*(pre_circle.radius- new_circle.radius))
        return INTERSECT;
}


// !at first, make sure that circle do not inside of every circle in the list_circle.
// !It means circle have intersection with anyone circle in the list_circle
int isThereIntersection(struct LIST_CIRCLE list_circle, struct CIRCLE new_circle)
{
    if(list_circle.length==0){centre_point = new_circle.centre; return 1;}

    free(tmp_list_circles.circles);
    tmp_list_circles.circles = NULL;
    tmp_list_circles.length = 0;

    int i;
    left = new_circle.centre.x - new_circle.radius;
    right = new_circle.centre.x + new_circle.radius;
    for(i=0; i<list_circle.length; i++)
    {
        switch(relationOf2Circles(list_circle.circles[i], new_circle)){
            case OUTSIDE: return 0;
            case INTERSECT:
                left = fmax(left, (list_circle.circles[i].centre.x-list_circle.circles[i].radius));
                right = fmin(right, (list_circle.circles[i].centre.x+list_circle.circles[i].radius));
                addCircleIntoList(&tmp_list_circles, list_circle.circles[i]);
                break;
            default: break;
        }
    }
    // for here not sure >= or onlt >
    if(left>right)
        return 0;

    addCircleIntoList(&tmp_list_circles, new_circle);

    return recursive1(left, right);
}

int recursive1(double left, double right) {

    int h_index_y=0, l_index_y=0;
    double mid;
    double square_dis, high_y, low_y, max_y, min_y;
    high_y = INFINITY;
    low_y = -INFINITY;

    while(left+0.5 < right)
    {
        mid = (left+right)/2;
        for(int i=0; i<tmp_list_circles.length;i++)
        {
            square_dis = (tmp_list_circles.circles[i].radius * tmp_list_circles.circles[i].radius)
                    - (mid-tmp_list_circles.circles[i].centre.x)*(mid-tmp_list_circles.circles[i].centre.x);

            max_y = tmp_list_circles.circles[i].centre.y + sqrt(square_dis);
            min_y  = tmp_list_circles.circles[i].centre.y - sqrt(square_dis);

            if(i==0 || max_y < high_y) {high_y = max_y;h_index_y = i;}
            if(i==0 || min_y > low_y) {low_y = min_y; l_index_y = i;}
        }

        if(high_y >= low_y)
        {
            if(recursive2(low_y, high_y, mid)) {
                return 1;
            }
        }

        double sqar_dis = (tmp_list_circles.circles[h_index_y].centre.x - tmp_list_circles.circles[l_index_y].centre.x)*(tmp_list_circles.circles[h_index_y].centre.x - tmp_list_circles.circles[l_index_y].centre.x)
                          + (tmp_list_circles.circles[h_index_y].centre.y - tmp_list_circles.circles[l_index_y].centre.y)*(tmp_list_circles.circles[h_index_y].centre.y - tmp_list_circles.circles[l_index_y].centre.y)
                          + (tmp_list_circles.circles[h_index_y].centre.z - tmp_list_circles.circles[l_index_y].centre.z)*(tmp_list_circles.circles[h_index_y].centre.z - tmp_list_circles.circles[l_index_y].centre.z);

        // x0 = x1 + (x2-x1)*(r1^2-r2^2+dis^2)/2dis^2
        double dis_AE = (tmp_list_circles.circles[l_index_y].radius*tmp_list_circles.circles[l_index_y].radius)
                        -(tmp_list_circles.circles[h_index_y].radius*tmp_list_circles.circles[h_index_y].radius)
                        +sqar_dis;
        double x0 = tmp_list_circles.circles[l_index_y].centre.x
                    + (tmp_list_circles.circles[h_index_y].centre.x - tmp_list_circles.circles[l_index_y].centre.x)
                      * dis_AE / (2*sqar_dis);

        if(x0 < mid)
            right = mid;
        else left = mid;
    }
    return 0;
}

int recursive2(double left, double right, double x)
{
    double y;
    double square_dis_z, high_z, low_z, max_z, min_z;
    int h_index_z=0, l_index_z=0;
    high_z = INFINITY;
    low_z = -INFINITY;
    double square_dis_to_line;
    while(left < right)
    {
        y = (left+right)/2;
        for(int i=0; i<tmp_list_circles.length;i++)
        {
            //todo
            square_dis_to_line = (x - tmp_list_circles.circles[i].centre.x)*(x - tmp_list_circles.circles[i].centre.x)
                        + (y - tmp_list_circles.circles[i].centre.y)*(y - tmp_list_circles.circles[i].centre.y);

            square_dis_z = (tmp_list_circles.circles[i].radius * tmp_list_circles.circles[i].radius)
                        - square_dis_to_line;

            max_z = tmp_list_circles.circles[i].centre.z + sqrt(square_dis_z);
            min_z  = tmp_list_circles.circles[i].centre.z - sqrt(square_dis_z);

            if(i==0 || max_z < high_z) {high_z = max_z;h_index_z = i;}
            if(i==0 || min_z > low_z) {low_z = min_z; l_index_z = i;}
        }

        if(high_z >= low_z)
        {
            // here can get point of n intersection y = 0.5*(high + low), x = mid
            centre_point.x = x;
            centre_point.y = y;
            centre_point.z = (high_z + low_z)/2;
            return 1;
        }

        double sqar_dis = (tmp_list_circles.circles[h_index_z].centre.x - tmp_list_circles.circles[l_index_z].centre.x)*(tmp_list_circles.circles[h_index_z].centre.x - tmp_list_circles.circles[l_index_z].centre.x)
                          + (tmp_list_circles.circles[h_index_z].centre.y - tmp_list_circles.circles[l_index_z].centre.y)*(tmp_list_circles.circles[h_index_z].centre.y - tmp_list_circles.circles[l_index_z].centre.y)
                          + (tmp_list_circles.circles[h_index_z].centre.z - tmp_list_circles.circles[l_index_z].centre.z)*(tmp_list_circles.circles[h_index_z].centre.z - tmp_list_circles.circles[l_index_z].centre.z);

        // x0 = x1 + (x2-x1)*(r1^2-r2^2+dis^2)/2dis^2
        double dis_AE = (tmp_list_circles.circles[l_index_z].radius*tmp_list_circles.circles[l_index_z].radius)
                        -(tmp_list_circles.circles[h_index_z].radius*tmp_list_circles.circles[h_index_z].radius)
                        +sqar_dis;
        double x0 = tmp_list_circles.circles[l_index_z].centre.y
                    + (tmp_list_circles.circles[h_index_z].centre.y - tmp_list_circles.circles[l_index_z].centre.y)
                      * dis_AE / (2*sqar_dis);

        if(x0 < y)
            right = y - 1;
        else left = y + 1;
    }
    return 0;
}

#endif //LTC_EU_CIRCLES_INTERSECT_H
