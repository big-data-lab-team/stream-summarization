//
// Created by boli on 10/10/18.
//

#ifndef NEBCOMPRESSMETHOD_COMPRESSIONENGINE_H
#define NEBCOMPRESSMETHOD_COMPRESSIONENGINE_H

#include <stdint-gcc.h>

#define NUMBER_OF_PARAMETER 3
#define WINDOW_SIZE 200

typedef struct {
    float epsilon; //in g, e.g., eps = 0.04 => 40mg
    uint16_t range; //accelerometer range in g, i.e., if range = 16 => 2^15 = 32768 => (16g)
}CompressionEngineConfig_t;

typedef struct {
    union {
        float data[3];
        struct {
            float dataX;
            float dataY;
            float dataZ;
        };
    };
    uint32_t timestamp;
} OutputData;



class CompressionEngine {
protected:
    typedef struct{
        uint32_t timestamp = 0;
        float params[NUMBER_OF_PARAMETER] = {0};
    }DataPoint;

    CompressionEngineConfig_t vCfg{};
    DataPoint base_data;
    DataPoint coming_data;
    uint32_t pre_timestamp=0;
    uint16_t count=0;

public:
    OutputData output{};
    virtual bool getCompressedData(uint32_t timestamp, int16_t x, int16_t y, int16_t z) = 0;
    virtual void reset() = 0;
    virtual bool isIntersect() = 0;
    bool init( const CompressionEngineConfig_t& prCfg ) {//todo
        vCfg = prCfg;
        return true;
    }

    CompressionEngine()
    {
        pre_timestamp = 0;
        count = 0;
    }
    virtual ~CompressionEngine() = default;
};



class InfinityCompression:public CompressionEngine
{

private:
    typedef struct {
        float upper[NUMBER_OF_PARAMETER] = {0};
        float lower[NUMBER_OF_PARAMETER] = {0};
    }BoundBox;

    BoundBox bound_box;
public:
    bool getCompressedData(uint32_t timestamp, int16_t x, int16_t y, int16_t z) override;
    void reset() override;
    bool isIntersect() override;

    void calculateOutput();

    InfinityCompression()
    {
        pre_timestamp = 0;
        count = 0;
    }
};


class EuclideanCompression: public CompressionEngine {
private:

    typedef struct{
        float center[NUMBER_OF_PARAMETER];
        float radius;
    }Circle;


    Circle correspond_circle;
//    Circle *circle_list;
    Circle circle_list[WINDOW_SIZE];
    uint8_t length_circle_list; //maybe duplicate with 'count'

    float intersect_point[NUMBER_OF_PARAMETER];
    float temp_point[NUMBER_OF_PARAMETER];

public:
    bool getCompressedData(uint32_t timestamp, int16_t x, int16_t y, int16_t z) override;
    void reset() override;
    bool isIntersect() override;

    void updateCorrespondCircle();
    void calculateOutput();
    int relationOfCircleAndCorrespcircle(const Circle &circle);
    int recursive(float left, float right, int index);

    EuclideanCompression()
    {
        pre_timestamp = 0;
        count = 0;
        length_circle_list = 0;
    }
};

inline float MAXf(float a, float b)
{
    return a >= b ? (a):(b);
}

inline float MINf(float a, float b)
{
    return a <= b ? (a):(b);
}

inline float ABSf(float x)
{
    if (x < 0)
        x = -x;
    return(x);
}


#endif //NEBCOMPRESSMETHOD_COMPRESSIONENGINE_H
