/**********************************************/





//provide a individual space to Read-write variable





/***********************************************/
#include "zf_common_headfile.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "IPM.h"
#include "Image_process.h"
#include "motor.h"
#include "ppersuit.h"
#include "encoder.h"
#include "pid.h"
#include "vofa.h"
#include "auto_drive.h"
#include "filter.h"
#include "cross.h"
#include "wifi.h"
#include "garage.h"
#include "ring.h"
#define POINTS_MAX_LEN 100

//Create image memory
uint8_t show0[RESULT_ROW][RESULT_COL];
uint8_t show1[RESULT_ROW][RESULT_COL];

//Build image structure

image_t img_raw={.data=show0[0], .width=RESULT_COL, .height=RESULT_ROW, .step=RESULT_COL};
image_t img_AP={.data=show1[0], .width=RESULT_COL, .height=RESULT_ROW, .step=RESULT_COL};


//Left and right information
AT_DTCM_SECTION_ALIGN(int ipts0[POINTS_MAX_LEN][2], 8);
AT_DTCM_SECTION_ALIGN(int ipts1[POINTS_MAX_LEN][2], 8);
//Counter of 
int ipts0_num, ipts1_num;


//Left and right information After Filter
AT_DTCM_SECTION_ALIGN(float rpts0b[POINTS_MAX_LEN][2], 8);
AT_DTCM_SECTION_ALIGN(float rpts1b[POINTS_MAX_LEN][2], 8);
//Counter of 
int rpts0b_num, rpts1b_num;
//


// Left and right information After reSample
AT_DTCM_SECTION_ALIGN(float rpts0s[POINTS_MAX_LEN][2], 8);
AT_DTCM_SECTION_ALIGN(float rpts1s[POINTS_MAX_LEN][2], 8);
//Counter of 
int rpts0s_num, rpts1s_num;


// Local angle variation rate of the sideline
AT_DTCM_SECTION_ALIGN(float rpts0a[POINTS_MAX_LEN], 8);
AT_DTCM_SECTION_ALIGN(float rpts1a[POINTS_MAX_LEN], 8);
//Counter of 
int rpts0a_num, rpts1a_num;


// Angle Change Rate Non-Maximum Suppression
AT_DTCM_SECTION_ALIGN(float rpts0an[POINTS_MAX_LEN], 8);
AT_DTCM_SECTION_ALIGN(float rpts1an[POINTS_MAX_LEN], 8);
int rpts0an_num, rpts1an_num;


// After the middle line tracking process
AT_DTCM_SECTION_ALIGN(float rptsc0[POINTS_MAX_LEN][2], 8);
AT_DTCM_SECTION_ALIGN(float rptsc1[POINTS_MAX_LEN][2], 8);
//Counter of 
int rptsc0_num, rptsc1_num;
//
//Long straight
bool is_straight0, is_straight1;
// L-corner
int Lpt0_rpts0s_id, Lpt1_rpts1s_id;
bool Lpt0_found, Lpt1_found;
//
//
extern int far_Lpt0_rpts0s_id, far_Lpt1_rpts1s_id;
extern int far_ipts0[FAR_POINTS_MAX_LEN][2];
extern int far_ipts1[FAR_POINTS_MAX_LEN][2];
extern int far_ipts0_num, far_ipts1_num;

extern float far_rpts0b[FAR_POINTS_MAX_LEN][2];
extern float far_rpts1b[FAR_POINTS_MAX_LEN][2];
extern int far_rpts0b_num, far_rpts1b_num;

extern float far_rpts0s[FAR_POINTS_MAX_LEN][2];
extern float far_rpts1s[FAR_POINTS_MAX_LEN][2];
extern int far_rpts0s_num, far_rpts1s_num;

extern float far_rpts0a[FAR_POINTS_MAX_LEN];
extern float far_rpts1a[FAR_POINTS_MAX_LEN];
extern int far_rpts0a_num, far_rpts1a_num;

extern float far_rpts0an[FAR_POINTS_MAX_LEN];
extern float far_rpts1an[FAR_POINTS_MAX_LEN];
extern int far_rpts0an_num, far_rpts1an_num;

extern int not_have_line;
//
float rpts[100][2];
int rpts_num;
//
extern enum track_type_e track_type;
//
float block_size = 7;
float clip_value = 2;
float thres = 120;
float begin_x =0;
float begin_y =91;
float line_blur_kernel=7;  
float sample_dist = 0.02;
float angle_dist = 0.4;
float pixel_per_meter = 104;
//y max ==115
//

//
//
extern float * aim0;
extern float * aim1;
//
//
extern int16 M1,M2,M3;
//
KFP KFP_height={0.01,0,0,0,0.005,0.103};
KFP KFP_angle={0.01,0,0,0,0.005,1.5};
//
float anglez=0;
//
float v;
pid_param_t m1=
{
	.error=0,
	.kd=0.45,//0.45,
	.ki=2,
	.kp=33,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};

pid_param_t m2=
{
	.error=0,
	.kd=0,//0.45,
	.ki=2,
	.kp=33,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};
pid_param_t m3=
{
	.error=0,
	.kd=0,//0.45,
	.ki=2,
	.kp=33,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};

pid_param_t Gangle=
{
	.error=0,
	.kd=0.001,
	.ki=0.00008,
	.kp=1.8,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};

pid_param_t Langle=
{
	.error=0,
	.kd=0.001,
	.ki=0.00008,
	.kp=1.8,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};

pid_param_t Rangle=
{
	.error=0,
	.kd=0.001,
	.ki=0.00008,
	.kp=1.8,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};

pid_param_t W=
{
	.error=0,
	.kd=0,
	.ki=0.1,
	.kp=1,
	.last_derivative=0,
	.last_error=0,
	.last_t=0,
	.out=0,
	.out_d=0,
	.out_i=0,
	.out_p=0
};