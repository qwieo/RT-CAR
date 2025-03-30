#ifndef __FILTER_H__
#define __FILTER_H__
#include "Image_process.h"
typedef struct 
{
    float LastP;
    float Now_P;
    float out;
    float Kg;
    float Q;
    float R;
}KFP;

float kalmanFilter(KFP *kfp,float input);
void gaussian(image_t *src, image_t *dst, int size, float sigma);
void blur(image_t *img0, image_t *img1, uint32_t kernel);
void minpool2(image_t *img0, image_t *img1);
double LowPass_gyro(float data_new, float data_last, float data_pre);
#endif