#include "zf_common_headfile.h"
#include "Image_process.h"
#include <math.h>
#define PREANCHOR0 5 
#define PREANCHOR1 5
extern float rptscs0[POINTS_MAX_LEN][2];
extern float rptscs1[POINTS_MAX_LEN][2];

float * aim0=rptscs0[PREANCHOR0];
float * aim1=rptscs1[PREANCHOR1];

float calculator(float * aim)
{
	float ld=sqrt((aim[0]-57.)*(aim[0]-57.)+aim[1]*aim[1]);
	if(abs(ld)<=1e-6)
	return 0;
	float sina=(aim[0]-57.)/ld;
	return -sina;
}

