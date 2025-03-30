#ifndef __PID_H__
#define	__PID_H__

typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //�����޷�
		float     					error;
    float                out_p;  //KP���
    float                out_i;  //KI���
    float                out_d;  //KD���
    float                out;    //pid���
    float                integrator; //< ����ֵ
    float                last_error; //< �ϴ����
    float                last_derivative;//< �ϴ���������ϴ����֮��
    unsigned long        last_t;     //< �ϴ�ʱ��
}pid_param_t;
//incremental 
float PidIncCtrl(pid_param_t * pid, float Encoder,float Target);
//positinal
float PidLocCtrl(pid_param_t * pid, float Encoder,float Target);

void PID_Init(pid_param_t * pid);

#endif

