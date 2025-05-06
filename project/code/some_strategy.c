#include "some_strategy.h"
int anchor0=5,anchor1=5;
uint8 preset=5;
void speed_strategy(float a)
{
	v=1-a	;
}

void preanchor_strategy(float v)
{
	anchor0=clip(preset+(int)(v-1.5)*20,0,rptsc0_num-1);	
	anchor1=clip(preset+(int)(v-1.5)*20,0,rptsc1_num-1);	
}
