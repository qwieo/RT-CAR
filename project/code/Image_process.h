#ifndef __ImageProcess_H__
#define __ImageProcess_H__
#include "zf_common_headfile.h"
#include "IPM.h"
#define USER_SIZE_H    100
#define USER_SIZE_W    114
#define User_Image    ImageUsed

// ensure vist image safely and correctly
// AT==safe(img[x][y].
#define AT_IMAGE(img, x, y)          ((img)->data[(y)*(img)->step+(x)])
#define AT                  AT_IMAGE
#define AT_CLIP(img, x, y)  AT_IMAGE((img), clip((x), 0, (img)->width-1), clip((y), 0, (img)->height-1));

#define DEF_IMAGE(ptr, w, h)         {.data=ptr, .width=w, .height=h, .step=w}

#define POINTS_MAX_LEN 100


#define white 255
#define black 0

typedef struct image {
    uint8_t *data;
    uint32_t width;
    uint32_t height;
    uint32_t step;
} image_t;

extern image_t img_raw;
extern image_t img_AP;

int clip(int x, int low, int up);
void draw_x(image_t *img, int x, int y, int len, uint8_t value);
void draw_o(image_t *img, int x, int y, int radius, uint8_t value);

void clone_image(image_t *img0, image_t *img1);

void clear_image(image_t *img);
//abandon
void adaptive_threshold
	(image_t *img0, image_t *img1, int block_size, int down_value, uint8_t low_value, uint8_t high_value);
// 3x3 expan,img0!=img1,the outermost pixel is not involved in the calculation
void dilate3(image_t *img0, image_t *img1);
// 3x3 erode,img0!=img1,The outermost pixel does not participate in the calculation
void erode3(image_t *img0, image_t *img1);


void draw(void);



//inverse perspective mapping
void IPMcopy(void);
//search for edge line 
void findline_lefthand_adaptive
	(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num);
void findline_righthand_adaptive
	(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num);
//edge line filter
void blur_points(int pts_in[][2], int num, float pts_out[][2], int kernel);
void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist);
//persuit center descrete point of left and right edge
void track_leftline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist);
void track_rightline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist);
//local slope
void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist);
//non-maximum inhibition after slope
void nms_angle(float angle_in[], int num, float angle_out[], int kernel);


extern uint8_t show[RESULT_ROW][RESULT_COL];
extern uint8_t show0[RESULT_ROW][RESULT_COL];
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
//Counter of 
extern int rpts0an_num, rpts1an_num;
extern float rpts[100][2];
extern int rpts_num;
extern int Lpt0_rpts0s_id, Lpt1_rpts1s_id;
extern bool Lpt0_found, Lpt1_found;

#endif
