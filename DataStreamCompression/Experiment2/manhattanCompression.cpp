

#include "compressionEngine.h"


bool ManhattanCompression::isIntersect()
{
    float time_differ = (float)count - 1;

    for(int param_index=0; param_index<NUMBER_OF_PARAMETER; param_index++)
    {
        //fabs(changed_x - (bound_box.max_x + bound_box.min_x)/2) > (bound_box.max_x-bound_box.min_x)/2 + EPSILON/ln_base
        if(ABSf(base_data.params[param_index] + (coming_data.params[param_index] - base_data.params[param_index])/time_differ - (bound_box.upper[param_index] + bound_box.lower[param_index])/2)
           >= (bound_box.upper[param_index] - bound_box.lower[param_index])/2 + vCfg.epsilon/time_differ)
            return false;
    }
    return true;
}
void ManhattanCompression::calculateOutput()
{
    output.timestamp = base_data.timestamp;
    output.dataX = base_data.params[0] * (32768/vCfg.range);
    output.dataY = base_data.params[1] * (32768/vCfg.range);
    output.dataZ = base_data.params[2] * (32768/vCfg.range);
}
bool ManhattanCompression::getCompressedData(uint32_t timestamp, int16_t x, int16_t y, int16_t z)
{
    count++;
    {
        coming_data.timestamp = timestamp;
        coming_data.params[0] = x/(float)(32768/vCfg.range);
        coming_data.params[1] = y/(float)(32768/vCfg.range);
        coming_data.params[2] = z/(float)(32768/vCfg.range);
    }
    if(count==1)
    {
        base_data = coming_data;
        pre_timestamp = coming_data.timestamp;

        output.timestamp = timestamp;
        output.dataX = x;
        output.dataY = y;
        output.dataZ = z;
        return true;
    }
    if(base_data.timestamp == pre_timestamp)
    {
        for (int i = 0; i < NUMBER_OF_PARAMETER; ++i)
        {
            bound_box.upper[i] = coming_data.params[i] + vCfg.epsilon;
            bound_box.lower[i] = coming_data.params[i] - vCfg.epsilon;
        }
        pre_timestamp = coming_data.timestamp;
        return false;
    }

    if(!isIntersect())// || count > 4*vCfg.rate
    {
        ///
        /// update base_data
        ///
        {
            float time_differ = (float)count - 2;
            base_data.timestamp = pre_timestamp;
            // new_x = ((bound_box.max_x + bound_box.min_x)/2 - base_data.ax)*time_differ + base_data.ax;
            for(int i=0; i<NUMBER_OF_PARAMETER; ++i)
            {
                base_data.params[i] += ((bound_box.upper[i] + bound_box.lower[i])/2 - base_data.params[i]) * time_differ;
            }
        }

        ///
        /// update output
        ///
        calculateOutput();

        ///
        ///	initial bounding box
        ///
        {
            for (int i = 0; i < NUMBER_OF_PARAMETER; ++i) {
                bound_box.upper[i] = coming_data.params[i] + vCfg.epsilon;
                bound_box.lower[i] = coming_data.params[i] - vCfg.epsilon;
            }
        }
        pre_timestamp = coming_data.timestamp;
        count = 2;

        return true;
    }

    ///
    /// update bounding box
    ///
    {
        float time_differ = (float)count - 1;
        //changed_param = (coming_data.ax-base_data.ax)/time_differ + base_data.ax;
        // bound_box_max = MIN(changed_param + EPSILON/time_differ, bound_box_max);
        for(int i=0; i<NUMBER_OF_PARAMETER; ++i)
        {
            bound_box.upper[i] = MINf(bound_box.upper[i],
                                      (base_data.params[i] + (coming_data.params[i] - base_data.params[i])/time_differ) + vCfg.epsilon/time_differ);
            bound_box.lower[i] = MAXf(bound_box.lower[i],
                                      (base_data.params[i] + (coming_data.params[i] - base_data.params[i])/time_differ) - vCfg.epsilon/time_differ);
        }
    }
    pre_timestamp = coming_data.timestamp;

    return false;
}
void ManhattanCompression::reset()
{
    if(count==0)
    {
        pre_timestamp = 0;
        return;
    }
    output.timestamp = coming_data.timestamp;

    for(int i=0; i<NUMBER_OF_PARAMETER; ++i)
    {
        output.data[i] = (32768/vCfg.range) * (base_data.params[i] + (count - 1)*((bound_box.upper[i] + bound_box.lower[i])/2.f - base_data.params[i]));
    }
    pre_timestamp = 0;
    count = 0;
}