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


// ��ͨOtsu�㷨������ֵ
uint8_t otsuThreshold(uint8_t *image, int Sumpix) {
    if (Sumpix <= 0) {
        return 0;  // ������Ч��Sumpixֵ
    }

    int *pixelCount = (int *)malloc(GrayScale * sizeof(int));
    float *pixelPro = (float *)malloc(GrayScale * sizeof(float));
    if (pixelCount == NULL || pixelPro == NULL) {
        // �ڴ����ʧ��
        return 0;
    }

    for (int i = 0; i < GrayScale; i++) {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint8_t threshold = 0;

    // ͳ��ÿ���Ҷȼ�����������
    for (int i = 0; i < Sumpix; i++) {
        if ((int)image[i] < GrayScale) {  // ����Ƿ�Խ��
            pixelCount[(int)image[i]]++;
        }
    }

    // ����ÿ���Ҷȼ������ظ���
    for (int i = 0; i < GrayScale; i++) {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;
    }

    float totalMean = 0;
    // ����ͼ�����ƽ���Ҷ�
    for (int i = 0; i < GrayScale; i++) {
        totalMean += i * pixelPro[i];
    }

    float maxVariance = 0.0;
    // �������п��ܵ���ֵ
    for (int t = 0; t < GrayScale; t++) {
        float w0 = 0; // ǰ�����ظ���
        float mean0 = 0; // ǰ������ƽ���Ҷ�
        for (int i = 0; i <= t; i++) {
            w0 += pixelPro[i];
            mean0 += i * pixelPro[i];
        }
        if (w0 > 0) {
            mean0 /= w0;
        }

        float w1 = 1 - w0; // �������ظ���
        if (w1 > 0) {  // ����Ƿ�Ϊ��
            float mean1 = (totalMean - w0 * mean0) / w1;

            // ������䷽��
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