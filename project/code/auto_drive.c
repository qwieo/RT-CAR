#include "zf_common_headfile.h"
#include "main.h"
#include "IPM.h"
#include "Image_process.h"
#include "motor.h"
#include "ppersuit.h"
#include "encoder.h"
#include "pid.h"
#include "vofa.h"
#define r 0.133
#define R 0.028
#define Tr 3.43

extern pid_param_t m1;
extern pid_param_t m2;
extern pid_param_t m3;
extern int16 M1,M2,M3;

void m1speed(float speed)
	{
		
	if ((int)PidIncCtrl(&m1,M1,speed)>=0)
	{
		gpio_low(C6);
		pwm_set_duty(Mo1,(int)PidIncCtrl(&m1,M1,speed));

	}
	else
	{
		gpio_high(C6);
		pwm_set_duty(Mo1,-(int)PidIncCtrl(&m1,M1,speed));
	}
		
	}
void m2speed(float speed)
	{
		
	if ((int)PidIncCtrl(&m2,M2,speed)>=0)
	{
		gpio_low(D2);
		pwm_set_duty(Mo2,(int)PidIncCtrl(&m2,M2,speed));

	}
	else
	{
		gpio_high(D2);
		pwm_set_duty(Mo2,-(int)PidIncCtrl(&m2,M2,speed));
	}
		
	}
	void m3speed(float speed)
	{
		
	if ((int)PidIncCtrl(&m3,M3,speed)>=0)
	{
		gpio_low(C8);
		pwm_set_duty(Mo3,(int)PidIncCtrl(&m3,M3,speed));

	}
	else
	{
		gpio_high(C8);
		pwm_set_duty(Mo3,-(int)PidIncCtrl(&m3,M3,speed));
	}
		
	}

void lyft_basic(float x,float y,float w)
{
	if(w!=w)
		w=0;
	float i=(y+r*w)*Tr/R;//(0.866*x-0.5*y+r*w)*Tr/R;
	float j=(-0.866*x-0.5*y+r*w)*Tr/R;//(y+r*w)*Tr/R;
	float k=(0.866*x-0.5*y+r*w)*Tr/R;
	m1speed(i);
	m2speed(j);
	m3speed(k); 
}