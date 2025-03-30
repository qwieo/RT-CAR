#include "zf_common_headfile.h"
#include "Image_process.h"
#include <math.h>
#define PREANCHOR0 26 //27 //
#define PREANCHOR1 27//27 //
extern float rptsc0[POINTS_MAX_LEN][2];
extern float rptsc1[POINTS_MAX_LEN][2];

float * aim0=rptsc0[PREANCHOR0];
float * aim1=rptsc1[PREANCHOR1];

float calculator(float * aim)
{
	float ld=sqrt((aim[0]-57)*(aim[0]-57)+aim[1]*aim[1]);
	if(abs(ld)<=1e-6)
	return 0;
	float sina=(aim[0]-57)/ld;
	float delta=atan(0.4*sina/(ld/100));
	return -delta;
}

