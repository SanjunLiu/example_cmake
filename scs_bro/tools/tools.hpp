#ifndef TOOLS_INCLUDE_
#define TOOLS_INCLUDE_

#include <iostream>
#include <string>
#include <string.h>  //memset
#include <iomanip>
#include <cmath>
#include <assert.h>

#if defined (LOG_TIME)
#include <ctime>
void getCurrentTime(struct timespec *time){
   clock_gettime(CLOCK_MONOTONIC_RAW, time);
}

double get_time(struct timespec start, struct timespec end){
    return end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) * 1e-9;
}
#endif

//init data
template<typename T>
inline void randomInitData(T *data, size_t size, T scale = 1) {
    clock_t ct = clock();
    srand((unsigned int)ct);
    for (size_t i=0; i<size; ++i)
        data[i] = ((T)rand())/RAND_MAX*scale;
}

template<typename T>
inline void constantInitData(T *data, T value, size_t size){
    if (value == 0)
        memset((void*)data, 0, size*sizeof(T));
    else
        for (size_t i=0; i<size; ++i)
            data[i] = value;
}

// read binary data
inline void readDataFromBinary(const char* fileName, const size_t size, float *data){
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL){
        printf("%s %d %s %s\n", __FILE__, __LINE__, fileName, " is not found");
    }
    size_t rs = fread(data, sizeof(float), size, fp);
    if(rs != size)
        std::cerr<<"read from the file is "<<rs<<" but the need size is "<<size<<std::endl;
    fclose(fp);
}

// write binary
inline void writeDataToBinary(const char* fileName, const size_t size, float *data){
    FILE *fp = fopen(fileName, "wb");
    size_t ws = fwrite(data, sizeof(float), size, fp);
    if(ws != size)
        std::cerr<<"write from the file is:"<<ws<<" but the need size is:"<<size<<std::endl;
    fclose(fp);
}

template<typename T>
void printNNumsData(size_t rows, size_t cols, const T *data){
    for(size_t i=0; i<rows; i++){
        for(size_t j=0; j<cols; j++){
            std::cout << data[i*cols+j] << ' ';
        }
        std::cout<<std::endl;
    }
}

#endif
