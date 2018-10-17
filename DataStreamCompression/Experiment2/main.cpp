#include <iostream>
#include <cmath>
#include "compressionEngine.h"

typedef struct{
    uint32_t timestamp = 0;
    int16_t x;
    int16_t y;
    int16_t z;
}DATA_POINT;

//#include "walk-local-data.c"
#include "run-local-data.c"

int main() {
    CompressionEngine *compressionEngine = nullptr;

    InfinityCompression infinityCompression;
//    EuclideanCompression euclideanCompression;

    CompressionEngineConfig_t vCfg = {(100/2048.f), (uint16_t)16};
    compressionEngine = &infinityCompression;
//    compressionEngine = &euclideanCompression;

    compressionEngine->init(vCfg);

    for(auto &data : data_list)
    {
        if(compressionEngine->getCompressedData(data.timestamp, data.x, data.y, data.z))
            std::cout << compressionEngine->output.timestamp << "," << compressionEngine->output.dataX << "," <<compressionEngine->output.dataY
                      << "," << compressionEngine->output.dataZ<< std::endl;
    }

    compressionEngine->reset();
    std::cout << compressionEngine->output.timestamp << "," << compressionEngine->output.dataX << "," <<compressionEngine->output.dataY
                << "," << compressionEngine->output.dataZ<< std::endl;

}
