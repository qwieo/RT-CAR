#include "zf_common_headfile.h"
#include "cross.h"
#ifndef RING_H
#define RING_H
#define POINTS_MAX_LEN 100
extern int ipts0[POINTS_MAX_LEN][2];
extern int ipts1[POINTS_MAX_LEN][2];
//
extern int ipts0_num, ipts1_num;
//
extern float rpts0b[POINTS_MAX_LEN][2];
extern float rpts1b[POINTS_MAX_LEN][2];
//
extern int rpts0b_num, rpts1b_num;
//
extern float rpts0s[POINTS_MAX_LEN][2];
extern float rpts1s[POINTS_MAX_LEN][2];
extern int rpts0s_num, rpts1s_num;
//
extern float rptsc0[POINTS_MAX_LEN][2];
extern float rptsc1[POINTS_MAX_LEN][2];
//
extern int rptsc0_num, rptsc1_num;
//
extern float rpts0a[POINTS_MAX_LEN];
extern float rpts1a[POINTS_MAX_LEN];
//Counter of 
extern int rpts0a_num, rpts1a_num;

// Angle Change Rate Non-Maximum Suppression
extern float rpts0an[POINTS_MAX_LEN];
extern float rpts1an[POINTS_MAX_LEN];

extern int rpts_num;
extern int rptsc0_num, rptsc1_num;
//
//Long straight
extern bool is_straight0, is_straight1;
// L-corner
extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;
extern bool Lpt0_found, Lpt1_found;

extern float sample_dist;
enum circle_type_e {
    CIRCLE_NONE = 0,                            // ��Բ��ģʽ
    CIRCLE_LEFT_BEGIN, CIRCLE_RIGHT_BEGIN,      // Բ����ʼ��ʶ�𵽵���L�ǵ���һ�೤ֱ����
    CIRCLE_LEFT_IN, CIRCLE_RIGHT_IN,            // Բ�����룬���ߵ�һ��ֱ����һ��Բ����λ�á�
    CIRCLE_LEFT_RUNNING, CIRCLE_RIGHT_RUNNING,  // Բ���ڲ���
    CIRCLE_LEFT_OUT, CIRCLE_RIGHT_OUT,          // ׼����Բ������ʶ�𵽳�������L�ǵ㡣
    CIRCLE_LEFT_END, CIRCLE_RIGHT_END,          // Բ�����������ٴ��ߵ�����ֱ����λ�á�
    CIRCLE_NUM,                                 //
};

extern enum circle_type_e circle_type;

void check_circle();

void run_circle();

void draw_circle();

#endif // CIRCLE_H