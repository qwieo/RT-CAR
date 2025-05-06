#include "box.h"
#include "Image_process.h"
#include "vofa.h"
uint8_t compaste_id;
enum box_position_type boxe=none_box;
bool LBF,RBF,MBF;
uint8_t LB_id,RB_id,MB_id;
int brpts0s_num,brpts1s_num;
void box_check(void){
	LBF=RBF=MBF=false;
	if(Lpt0_found)
		brpts0s_num=Lpt0_rpts0s_id;
	else
		brpts0s_num=rpts0s_num;	
	if(Lpt1_found)
		brpts1s_num=Lpt1_rpts1s_id;
	else
		brpts1s_num=rpts1s_num;	
	
	
	if(brpts0s_num>=brpts1s_num)
	{
		for(uint8_t i=0;i<brpts0s_num;i++)
		{
			if(MBF)
				break;
			for(int j=-3;j<4;j++)
			{
				if(AT_IMAGE(&img_AP,(int)rptsc0[i][0]+j,(int)rptsc0[i][1])<=140){
					compaste_id=i;
					boxe=left_box;
					MBF=true;
					break;}
			}
		}
	}
	
	else
		
	{
		for(uint8_t i=0;i<brpts1s_num;i++)
		{
			if(MBF)
				break;
			for(int j=-3;j<4;j++)
			{
				if(AT_IMAGE(&img_AP,(int)rptsc1[i][0]+j,(int)rptsc1[i][1])<=140){
					compaste_id=i;
					boxe=right_box;
					MBF=true;
					break;}
			}
		}
	}
	if(!MBF)
	{
			if(Lpt0_found&&!Lpt1_found){
				LBF=true;
				boxe=left_box;
				compaste_id=Lpt0_rpts0s_id;}
			if(Lpt1_found&&!Lpt0_found){
				RBF=true;
				boxe=right_box;
				compaste_id=Lpt1_rpts1s_id;}
	}	
}
float angle_compaste(void){
	float dx;
	float dy;
	float t0;
	float t1;
	float t;
	if(!(LBF||RBF||MBF))
		return 0.f;
	switch(boxe)
	{
		case none_box	:
			break;		
		case left_box	:
			dx = rpts0s[compaste_id][0]-rpts0s[0][0];
			dy = rpts0s[compaste_id][1]-rpts0s[0][1];
			t0 = atan(dx/dy);
//			dx = rptsc0[0][0]-rpts0s[0][0];
//			dy = rptsc0[0][1]-rpts0s[0][1];
//			t1 = atan(dy/dx);
			dx = rpts0s[2][0]-rpts0s[0][0];
			dy = rpts0s[2][0]-rpts0s[0][1];
			t1 = atan(dx/dy);
			t  = 2*t0+3*t1;
			break;
		
		case right_box	:
			dx = rpts1s[compaste_id][0]-rpts1s[2][0];
			dy = rpts1s[compaste_id][1]-rpts1s[2][1];
			t0 = atan(dx/dy);
//			dx = rptsc1[0][0]-rpts1s[0][0];
//			dy = rptsc1[0][1]-rpts1s[0][1];
//			t1 = atan(dy/dx);
			dx = rpts1s[2][0]-rpts1s[0][0];
			dy = rpts1s[2][0]-rpts1s[0][1];
			t1 = atan(dx/dy);
			t  = 2*t0+3*t1;
			break;	
	}
	return t;
}

