#ifndef __PID_H__
#define	__PID_H__

typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //积分限幅
		float     					error;
    float                out_p;  //KP输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out;    //pid输出
    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次误差与上上次误差之差
    unsigned long        last_t;     //< 上次时间
}pid_param_t;
//incremental 
float PidIncCtrl(pid_param_t * pid, float Encoder,float Target);
//positinal
float PidLocCtrl(pid_param_t * pid, float Encoder,float Target);

void PID_Init(pid_param_t * pid);

#endif

