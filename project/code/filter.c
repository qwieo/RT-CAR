#include "zf_common_headfile.h"
#include "filter.h"

float kalmanFilter(KFP *kfp,float input)
{

    kfp->Now_P = kfp->LastP + kfp->Q;
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    kfp->out = kfp->out + kfp->Kg * (input -kfp->out);
    kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
    return kfp->out;
}

void get_gau_kernel(float **kernel, int size, float sigma)
{
	if (size <= 0 || fabs(sigma)< 0.0001)
		return;
 
	int x, y;
	int m = size / 2;
	float sum = 0;
 
	//get kernel
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			kernel[y][x] = (1 / (2 * PI * sigma * sigma)) * exp(-((x - m) * (x - m) + (y - m) * (y - m)) / (2 * sigma * sigma));
			sum += kernel[y][x];
		}
	}
 
	//normal
	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			kernel[y][x] /= sum;
		}
	}
}
 
 

void gaussian(image_t *src, image_t *dst, int size, float sigma)
{
	if (src->width == 0 || src->height == 0)
		return;
 
	int y, x;
	int i, j;
	int m = size / 2;
	float value;
 
	float **kernel = (float**)malloc(size * sizeof(float*));
	for (i = 0; i < size; i++)
		kernel[i] = (float*)malloc(size * sizeof(float));
 
	get_gau_kernel(kernel,size,sigma);
 
	float *kernel_vec = (float*)malloc(size * size * sizeof(float));
 
	int k = 0;
	for (j = 0; j < size; j++)
	{
		for (i = 0; i < size; i++)
		{
			kernel_vec[k++] = kernel[j][i];
		}
	}
 
	uint8_t *src_ptr = src->data + m * src -> width;
	uint8_t *dst_ptr = dst->data + m * dst -> width;
	for (y = m; y < src -> height - m ; y++)
	{
		for (x = m; x < src->width - m; x++)
		{
 
			value = 0;
			k = 0;
			for (j = -m; j < m;j++)
			{
				for (i = -m; i < m; i++)
				{
					char temp = src_ptr[(y + j) * src->width + (x + i)];
					float temp1 = kernel_vec[k++];
					value += temp * temp1;
				}
			}
 
			dst_ptr[x] = (char)(value);
		}
 
		dst_ptr += dst->width;
	}
 
	free(kernel_vec);
	for (i = 0; i < size; i++)
		free(kernel[i]);
	free(kernel);
}

AT_ITCM_SECTION_INIT(void blur(image_t *img0, image_t *img1, uint32_t kernel)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
                    AT(img1, x, y) = (1 * AT(img0, x - 1, y - 1) + 2 * AT(img0, x, y - 1) + 1 * AT(img0, x + 1, y - 1) +
                                      2 * AT(img0, x - 1, y) + 4 * AT(img0, x, y) + 2 * AT(img0, x + 1, y) +
                                      1 * AT(img0, x - 1, y + 1) + 2 * AT(img0, x, y + 1) + 1 * AT(img0, x + 1, y + 1)) / 16;
        }
    }
}
AT_ITCM_SECTION_INIT(void minpool2(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width / 2 == img1->width && img0->height / 2 == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    uint8_t min_value;
    for (int y = 1; y < img0->height; y += 2) {
        for (int x = 1; x < img0->width; x += 2) {
            min_value = 255;
            if (AT(img0, x, y) < min_value) min_value = AT(img0, x, y);
            if (AT(img0, x - 1, y) < min_value) min_value = AT(img0, x - 1, y);
            if (AT(img0, x, y - 1) < min_value) min_value = AT(img0, x, y - 1);
            if (AT(img0, x - 1, y - 1) < min_value) min_value = AT(img0, x - 1, y - 1);
                    AT(img1, x / 2, y / 2) = min_value;
        }
    }
}


double LowPass_gyro(float data_new, float data_last, float data_pre)
{
    return (0.5f * data_new + 0.3f * data_last + 0.2f * data_pre);
}