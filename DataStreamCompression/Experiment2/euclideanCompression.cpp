//
// Created by boli on 10/1/18.
//

#include "compressionEngine.h"
#include <math.h>
#define OUTSIDE 0
#define INTERSECT 1
#define INSIDE -1


void EuclideanCompression::calculateOutput()
{
    output.timestamp = base_data.timestamp;
    output.dataX = base_data.params[0] * (32768/vCfg.range);
    output.dataY = base_data.params[1] * (32768/vCfg.range);
    output.dataZ = base_data.params[2] * (32768/vCfg.range);
}

bool EuclideanCompression::getCompressedData(uint32_t timestamp, int16_t x, int16_t y, int16_t z) {
    count++;
    {
        coming_data.timestamp = timestamp;
        coming_data.params[0] = x/(float)(32768/vCfg.range);
        coming_data.params[1] = y/(float)(32768/vCfg.range);
        coming_data.params[2] = z/(float)(32768/vCfg.range);
    }
    if(count == 1)
    {
        base_data = coming_data;
        pre_timestamp = coming_data.timestamp;

        output.timestamp = timestamp;
        output.dataX = x;
        output.dataY = y;
        output.dataZ = z;
        return true;
    }

    // update correspond circle
    updateCorrespondCircle();

    if(length_circle_list == 0)
    {
        circle_list[length_circle_list++] = correspond_circle;

        //update output_data = correspond_circle.center
        for (int i = 0; i < NUMBER_OF_PARAMETER; ++i) {
            intersect_point[i] = correspond_circle.center[i];
        }
        pre_timestamp = coming_data.timestamp;

        return false;
    }

    if(length_circle_list >= WINDOW_SIZE || !isIntersect() )
    {
        ///
        /// update base_point
        ///
        {
            for(int i=0; i<NUMBER_OF_PARAMETER; i++)
            {
                base_data.params[i] += (intersect_point[i] - base_data.params[i]) * (float)(count-2);
            }
            base_data.timestamp = pre_timestamp;
        }

        ///
        /// update output
        ///
        calculateOutput();

        ///
        /// Initial length_circle_list, and count = 2;
        ///
        length_circle_list = 0;
        count = 2;

        //update correspond circle, case the base point has been changed so it needs compute again.
        updateCorrespondCircle();

        ///
        /// add current correspond circle into circle_list (second point == the first point after base point)
        /// and length_circle_list+=1
        ///
        circle_list[length_circle_list++] = correspond_circle;

        ///
        /// update output_data = correspond_circle.center
        ///
        {
            for (int i = 0; i < NUMBER_OF_PARAMETER; ++i)
            {
                intersect_point[i] = correspond_circle.center[i];
            }

        }
        pre_timestamp = coming_data.timestamp;
        return true;
    }

    pre_timestamp = coming_data.timestamp;
    return false;
}

void EuclideanCompression::updateCorrespondCircle()
{
    correspond_circle.radius = vCfg.epsilon/(float)(count-1);
    for (int i = 0; i < NUMBER_OF_PARAMETER; ++i) {
        correspond_circle.center[i] = base_data.params[i] + (coming_data.params[i] - base_data.params[i])/(float)(count-1);
    }
}

bool EuclideanCompression::isIntersect() {
    int param_index = NUMBER_OF_PARAMETER - 1;

    float left = correspond_circle.center[param_index] - correspond_circle.radius;
    float right = correspond_circle.center[param_index] + correspond_circle.radius;

    ///
    /// for each circle in circle_list, check the relation with correspond_circle,
    /// if there is one circle dont intersect, return false. else if include correspond_circle remove this circle in list
    ///

    for(int i=0; i<length_circle_list; ++i)
    {
        switch(relationOfCircleAndCorrespcircle(circle_list[i])){
            case OUTSIDE:
                return false;
            case INTERSECT:
                left = MAXf(left, circle_list[i].center[param_index] - circle_list[i].radius);
                right = MINf(right, circle_list[i].center[param_index] + circle_list[i].radius);
                break;
            default:
                //should remove the circle which include correspond_circle;
                break;
        }
    }

    circle_list[length_circle_list++] = correspond_circle;
    // gonna remove follow for loop
//    for(auto current_circle = list_circle.begin(); current_circle != list_circle.end(); )
//    {
//        switch (relationOfCircleAndCorrespcircle(*current_circle)) {
//            case OUTSIDE:
//                return false;
//            case INTERSECT:
//                left = max(left, ((*current_circle).center[param_index] - (*current_circle).radius));
//                right = min(right, ((*current_circle).center[param_index] + (*current_circle).radius));
//                current_circle++;
//                break;
//            default:
//                current_circle = list_circle.erase(current_circle);
//        }
//    }

    if (left > right)
        return false;

    if(recursive(left, right, param_index) == 1){
        for (int i = 0; i < NUMBER_OF_PARAMETER; ++i) {
            intersect_point[i] = temp_point[i];
        }
        return true;
    }else{
        return false;
    }
}

int EuclideanCompression::relationOfCircleAndCorrespcircle(const EuclideanCompression::Circle &circle) {
    float square_dis=0;
    for (int i = 0; i < NUMBER_OF_PARAMETER; i++) {
        square_dis += (circle.center[i] - correspond_circle.center[i])
                      *(circle.center[i] - correspond_circle.center[i]);
    }
    if(square_dis > (circle.radius + correspond_circle.radius)*(circle.radius + correspond_circle.radius))
        return OUTSIDE;
    else if (square_dis > (circle.radius - correspond_circle.radius)*(circle.radius- correspond_circle.radius))
        return INTERSECT;
    else return INSIDE;
}

int EuclideanCompression::recursive(float left, float right, int index) {
    if(index ==0)
    {
        temp_point[index] = (left+right)/2;
        return  1;
    }

    uint8_t h_index=0, l_index=0;
    float mid;
    float high, low, max, min, square_dis=0, square_dis_to_line=0;
//    high = INFINITY;
//    low = -INFINITY;

    while(left < right)
    {
        mid = (left+right)/2;

        for(int i=0; i<length_circle_list; ++i)
        {

            square_dis_to_line = (mid - circle_list[i].center[index]) * (mid - circle_list[i].center[index]);
            for(int j = index+1; j<NUMBER_OF_PARAMETER; j++)
            {
                square_dis_to_line += (temp_point[j] - circle_list[i].center[j]) * (temp_point[j] - circle_list[i].center[j]);
            }

            square_dis = (circle_list[i].radius * circle_list[i].radius) - square_dis_to_line;

            max = circle_list[i].center[index-1] + sqrt(square_dis);
            min  = circle_list[i].center[index-1] - sqrt(square_dis);

            if(i==0 || max < high) {high = max; h_index = i;}
            if(i==0 || min > low) {low = min; l_index = i;}
        }

        if(high >= low)
        {
            temp_point[index] = mid;
            if(recursive(low, high, index-1)) {
                temp_point[index] = mid;
                return 1;
            }
        }

        square_dis = 0;
        for(int i=0; i<NUMBER_OF_PARAMETER; ++i)
        {
            square_dis += (circle_list[h_index].center[i] - circle_list[l_index].center[i])
                          *(circle_list[h_index].center[i] - circle_list[l_index].center[i]);
        }

        // x0 = x1 + (x2-x1)*(r1^2-r2^2+dis^2)/2dis^2
        float dis_AE = (circle_list[l_index].radius * circle_list[l_index].radius)
                       - (circle_list[h_index].radius*circle_list[h_index].radius) + square_dis;

        float intersect_point = circle_list[l_index].center[index]
                                + (circle_list[h_index].center[index] - circle_list[l_index].center[index])
                                  * dis_AE / (2*square_dis);

        if(intersect_point < mid)
            right = mid - 1/2048.f;
        else left = mid + 1/2048.f;
    }
    return 0;
}

void EuclideanCompression::reset()
{
    if(count==0)
    {
        pre_timestamp = 0;
        length_circle_list = 0;
        return;
    }
    output.timestamp = coming_data.timestamp;

    for(int i=0; i<NUMBER_OF_PARAMETER; ++i)
    {
        output.data[i] = (32768/vCfg.range) * (base_data.params[i] + (count - 1)*(intersect_point[i] - base_data.params[i]));
    }
    length_circle_list = 0;
    pre_timestamp = 0;
    count = 0;
}

