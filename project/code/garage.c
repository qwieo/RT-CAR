#include "garage.h"
#include <math.h>
#include <stdlib.h>  
#define GrayScale 256

uint8_t pixels[114];

bool check_zebra(void)
{
	static uint8 f=1,lf=1;
	uint8 cnt=0,thres;
	uint8 j=0;	
	for(uint8 i=0;i<114;i++)
		{
			pixels[i]=AT(&img_raw, i,90);
		}
		thres = otsuThreshold(pixels,114);
		for(uint8 i=0;i<114;i++)
		{
			if(AT(&img_raw, i,90)<=140)
				f=0;
			else
				f=1;
			if(lf!=f)
				cnt++;
			lf=f;			
		}				
	if(cnt>=9)
	return true;
	else
	return false;
}


// 普通Otsu算法计算阈值
uint8_t otsuThreshold(uint8_t *image, int Sumpix) {
    if (Sumpix <= 0) {
        return 0;  // 处理无效的Sumpix值
    }

    int *pixelCount = (int *)malloc(GrayScale * sizeof(int));
    float *pixelPro = (float *)malloc(GrayScale * sizeof(float));
    if (pixelCount == NULL || pixelPro == NULL) {
        // 内存分配失败
        return 0;
    }

    for (int i = 0; i < GrayScale; i++) {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint8_t threshold = 0;

    // 统计每个灰度级的像素数量
    for (int i = 0; i < Sumpix; i++) {
        if ((int)image[i] < GrayScale) {  // 检查是否越界
            pixelCount[(int)image[i]]++;
        }
    }

    // 计算每个灰度级的像素概率
    for (int i = 0; i < GrayScale; i++) {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;
    }

    float totalMean = 0;
    // 计算图像的总平均灰度
    for (int i = 0; i < GrayScale; i++) {
        totalMean += i * pixelPro[i];
    }

    float maxVariance = 0.0;
    // 遍历所有可能的阈值
    for (int t = 0; t < GrayScale; t++) {
        float w0 = 0; // 前景像素概率
        float mean0 = 0; // 前景像素平均灰度
        for (int i = 0; i <= t; i++) {
            w0 += pixelPro[i];
            mean0 += i * pixelPro[i];
        }
        if (w0 > 0) {
            mean0 /= w0;
        }

        float w1 = 1 - w0; // 背景像素概率
        if (w1 > 0) {  // 检查是否为零
            float mean1 = (totalMean - w0 * mean0) / w1;

            // 计算类间方差
            float variance = w0 * w1 * pow((mean0 - mean1), 2);

            if (variance > maxVariance) {
                maxVariance = variance;
                threshold = t;
            }
        }
    }

    free(pixelCount);
    free(pixelPro);

    return threshold;
}