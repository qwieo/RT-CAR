#include "zf_common_headfile.h"
#define Mo1		PWM2_MODULE0_CHB_C7//PWM2_MODULE1_CHA_C8
#define Mo2		PWM1_MODULE3_CHA_D0//PWM2_MODULE3_CHA_D2
#define Mo3		PWM2_MODULE1_CHB_C9
void Motor_Init(void)
{			
		pwm_init(Mo1, 17000, 0);                                          
		gpio_init(C6,GPO,GPIO_LOW,GPO_PUSH_PULL);
		pwm_init(Mo2, 17000, 0);
    gpio_init(D2,GPO,GPIO_LOW,GPO_PUSH_PULL);
		pwm_init(Mo3, 17000, 0);
		gpio_init(C8,GPO,GPIO_LOW,GPO_PUSH_PULL);
}

	
