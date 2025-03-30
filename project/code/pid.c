#include  "zf_common_headfile.h"
#include "pid.h"

#define  Max 6500
#define Data_Max  6500
#define N 6.2
#define dt 0.005
/*
InitializePID Structure PID 
*/
/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid增量式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float PidIncCtrl(pid_param_t * pid, float Encoder,float Target)
{

		pid->error=(float)(Target - Encoder);//当前误差
    pid->out_p = pid->kp * (pid->error - pid->last_error);

		//printf("%f",pid->out_p);
    pid->out_i = pid->ki * pid->error;
    pid->out_d = pid->kd * ((pid->error - 2*(pid->last_error)) + pid->last_derivative);

    pid->last_derivative = pid->error - pid->last_error;
    pid->last_error = pid->error;

    pid->out += pid->out_p + pid->out_i + pid->out_d;
//		pid->error=(float)(Target - Encoder);//当前误差
//    pid->out_p = pid->kp*pid->error;// * (pid->error - pid->last_error);

//		//printf("%f",pid->out_p);
//    pid->out_i+= pid->ki * pid->error*dt;
//		pid->out_d=(1.0f-N *dt)*pid->last_derivative + pid->kd*N*(Target-pid->last_error);
//    pid->last_error =Target;
//    pid->out = pid->out_p + pid->out_i+pid->out_d;
//		pid->last_derivative=pid->out_d;
    return pid->out=func_limit_ab(pid->out, -Max, Max);
}


/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid位置式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float PidLocCtrl(pid_param_t * pid, float Encoder,float Target)
{
	
	pid->error=(float)(Target - Encoder);//当前误差
    /* 累积误差 */
    pid->integrator += pid->error;

    /* 误差限幅 */
   // constrain_float(pid->integrator, -pid->imax, pid->imax);


    pid->out_p = pid->kp * pid->error;
    pid->out_i = pid->ki * pid->integrator;
    pid->out_d = pid->kd * (pid->error - pid->last_error);

    pid->last_error = pid->error;

    pid->out = pid->out_p + func_limit_ab(pid->out_i,-0.5,0.5) + pid->out_d;

    return pid->out=func_limit_ab(pid->out, -Max, Max);
}

void PID_Init(pid_param_t * pid)
{
	
	pid->out=0.;
	pid->out_d=0.;
	pid->out_i=0.;
	pid->out_p=0.;
	pid->last_error=0.;
	pid->last_derivative=0.;
	pid->last_t=0.;
	pid->integrator=0.;
}

