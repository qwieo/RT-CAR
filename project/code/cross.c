#include "zf_common_headfile.h"
#include "cross.h"
#include "encoder.h"
#include "Image_process.h"
#define img_line img_AP
enum cross_type_e cross_type = CROSS_NONE;
enum track_type_e track_type = TRACK_MIX;
extern image_t img_AP;


bool line_show_sample = true;
bool far_Lpt0_found, far_Lpt1_found;
int far_Lpt0_rpts0s_id, far_Lpt1_rpts1s_id;


int far_ipts0[FAR_POINTS_MAX_LEN][2];
int far_ipts1[FAR_POINTS_MAX_LEN][2];
int far_ipts0_num, far_ipts1_num;

float far_rpts0b[FAR_POINTS_MAX_LEN][2];
float far_rpts1b[FAR_POINTS_MAX_LEN][2];
int far_rpts0b_num, far_rpts1b_num;

float far_rpts0s[FAR_POINTS_MAX_LEN][2];
float far_rpts1s[FAR_POINTS_MAX_LEN][2];
int far_rpts0s_num, far_rpts1s_num;

float far_rpts0a[FAR_POINTS_MAX_LEN];
float far_rpts1a[FAR_POINTS_MAX_LEN];
int far_rpts0a_num, far_rpts1a_num;

float far_rpts0an[FAR_POINTS_MAX_LEN];
float far_rpts1an[FAR_POINTS_MAX_LEN];
int far_rpts0an_num, far_rpts1an_num;

int not_have_line = 0;

int far_x1 = 52, far_x2 = 70, far_y1, far_y2;


extern float block_size;
extern float clip_value;
extern float sample_dist;
extern float begin_x;
extern float begin_y;
extern float thres;
extern float line_blur_kernel;  
extern float angle_dist;
extern float pixel_per_meter;


void check_cross() {
    bool Xfound = Lpt0_found && Lpt1_found;
    if (cross_type == CROSS_NONE && Xfound) 
		cross_type = CROSS_BEGIN;
}

void run_cross() {
    bool Xfound = Lpt0_found && Lpt1_found;
    float Lpt0y = rpts0s[Lpt0_rpts0s_id][1];
    float Lpt1y = rpts1s[Lpt1_rpts1s_id][1];
    if (cross_type == CROSS_BEGIN) {
        if (Lpt0_found) {
            rptsc0_num = rpts0s_num = Lpt0_rpts0s_id;
        }
        if (Lpt1_found) {
            rptsc1_num = rpts1s_num = Lpt1_rpts1s_id;
        }


        if ((Xfound && (Lpt0_rpts0s_id < 0.1 / sample_dist || Lpt1_rpts1s_id < 0.1 / sample_dist))|| (ipts1_num <5 && ipts0_num<5)) {
            cross_type = CROSS_IN;
            
        }
    }
    else if (cross_type == CROSS_IN) {
        cross_farline();

        if (rpts1s_num < 40 && rpts0s_num < 40) { not_have_line++; }
        if (not_have_line > 2 && rpts1s_num > 40 && rpts0s_num > 40) {
            cross_type = CROSS_NONE;
            not_have_line = 0;
        }
				
        if (far_Lpt1_found) { track_type = TRACK_RIGHT; }
        else if (far_Lpt0_found) { track_type = TRACK_LEFT; }
        else if (not_have_line > 0 && rpts1s_num < 5) { track_type = TRACK_RIGHT; }
        else if (not_have_line > 0 && rpts0s_num < 5) { track_type = TRACK_LEFT; }

    }
}

// Cross debug image
void draw_cross() {
    if (cross_type == CROSS_IN && line_show_sample) {
        for (int i = 0; i < far_rpts0s_num; i++) {
            AT_IMAGE(&img_line, clip(far_rpts0s[i][0], 0, 113), clip(far_rpts0s[i][1], 0,99)) = 100;
        }
        for (int i = 0; i < far_rpts1s_num; i++) {
            AT_IMAGE(&img_line, clip(far_rpts1s[i][0], 0, 113), clip(far_rpts1s[i][1], 0,99)) = 100;
        }

        if (far_Lpt0_found) {
            draw_x(&img_line, far_rpts0s[far_Lpt0_rpts0s_id][0], far_rpts0s[far_Lpt0_rpts0s_id][1], 5, 255);
				}
				
        if (far_Lpt1_found) {
            draw_x(&img_line, far_rpts1s[far_Lpt1_rpts1s_id][0], far_rpts1s[far_Lpt1_rpts1s_id][1], 5, 255);
				}
			
				draw_o(&img_line, clip(far_x1, 0, img_line.width - 1), clip((int) begin_y, 0, img_line.height - 1), 3, 255);
        draw_o(&img_line, clip(far_x2, 0, img_line.width - 1), clip((int) begin_y, 0, img_line.height - 1), 3, 255);
        draw_o(&img_line, clip(far_x1, 0, img_line.width - 1), clip(far_y1, 0, img_line.height - 1), 3, 255);
        draw_o(&img_line, clip(far_x2, 0, img_line.width - 1), clip(far_y2, 0, img_line.height - 1), 3, 255);

    }
}

void cross_farline() {
    int cross_width = 6;
//    far_x1 = cross_width, far_x2 = img_raw.width -cross_width;
    far_y1 = 0, far_y2 = 0;


    int x1 = img_AP.width / 2 - begin_x, y1 = begin_y;
    bool white_found = false;
    far_ipts0_num = sizeof(far_ipts0) / sizeof(far_ipts0[0]);

    for(;x1>cross_width*2; x1--) 
    {
      if(AT_IMAGE(&img_AP, x1-1, y1) < thres) {
        far_x1 = x1 - cross_width;
        break;
      }   
    }

    for (; y1 > 0; y1--) {

        if (AT_IMAGE(&img_AP, far_x1, y1) >= thres) { white_found = true; }
        if (AT_IMAGE(&img_AP, far_x1, y1) < thres && (white_found || far_x1 == cross_width)) {
            far_y1 = y1;
            break;
        }
    }


    if (AT_IMAGE(&img_AP, far_x1, far_y1 +1) >= thres)
		{findline_lefthand_adaptive(&img_AP, block_size, clip_value, far_x1, far_y1+1, far_ipts0, &far_ipts0_num);
		 ips200_show_uint(160,160,far_ipts0_num,3);
		}
			else far_ipts0_num = 0;

    int x2 = img_AP.width / 2 + begin_x, y2 = begin_y;
    white_found = false;
    far_ipts1_num = sizeof(far_ipts1) / sizeof(far_ipts1[0]);


    for(;x2<img_AP.width-cross_width*2; x2++) 
    {
      if(AT_IMAGE(&img_AP, x2+1, y2) < thres) {
        far_x2 = x2 + cross_width;
        break;
      }   
    }

    for (; y2 > 0; y2--) {

        if (AT_IMAGE(&img_AP, far_x2, y2) >= thres) { white_found = true; }
        if (AT_IMAGE(&img_AP, far_x2, y2) < thres && (white_found || far_x2 == img_AP.width - cross_width)) {
            far_y2 = y2;
            break;
        }
    }


    if (AT_IMAGE(&img_AP, far_x2, far_y2 + 1) >= thres)
		{findline_righthand_adaptive(&img_AP, block_size, clip_value, far_x2, far_y2 + 1, far_ipts1, &far_ipts1_num);
		 ips200_show_uint(180,180,far_ipts1_num,3);
		}
			
		
		else far_ipts1_num = 0;





    blur_points(far_ipts0, far_ipts0_num, far_rpts0b, (int) round(line_blur_kernel));
    far_rpts0b_num = far_ipts0_num;
    blur_points(far_ipts1, far_ipts1_num, far_rpts1b, (int) round(line_blur_kernel));
    far_rpts1b_num = far_ipts1_num;


    far_rpts0s_num = sizeof(far_rpts0s) / sizeof(far_rpts0s[0]);
    resample_points(far_rpts0b, far_rpts0b_num, far_rpts0s, &far_rpts0s_num, sample_dist * pixel_per_meter);
    far_rpts1s_num = sizeof(far_rpts1s) / sizeof(far_rpts1s[0]);
    resample_points(far_rpts1b, far_rpts1b_num, far_rpts1s, &far_rpts1s_num, sample_dist * pixel_per_meter);

    local_angle_points(far_rpts0s, far_rpts0s_num, far_rpts0a, (int) round(angle_dist / sample_dist));
    far_rpts0a_num = far_rpts0s_num;
    local_angle_points(far_rpts1s, far_rpts1s_num, far_rpts1a, (int) round(angle_dist / sample_dist));
    far_rpts1a_num = far_rpts1s_num;


    nms_angle(far_rpts0a, far_rpts0a_num, far_rpts0an, (int) round(angle_dist / sample_dist) * 2 + 1);
    far_rpts0an_num = far_rpts0a_num;
    nms_angle(far_rpts1a, far_rpts1a_num, far_rpts1an, (int) round(angle_dist / sample_dist) * 2 + 1);
    far_rpts1an_num = far_rpts1a_num;

    far_Lpt0_found = far_Lpt1_found = false;
    for (int i = 0; i < MIN(far_rpts0s_num, 80); i++) {
        if (fabs(far_rpts0an[i])<0.0001) continue;
        int im1 = clip(i - (int) round(angle_dist / sample_dist), 0, far_rpts0s_num - 1);
        int ip1 = clip(i + (int) round(angle_dist / sample_dist), 0, far_rpts0s_num - 1);
        float conf = fabs(far_rpts0a[i]) - (fabs(far_rpts0a[im1]) + fabs(far_rpts0a[ip1])) / 2;
        if (45. / 180. * PI < conf && conf < 110. / 180. * PI && i < 100) {
            far_Lpt0_rpts0s_id = i;
            far_Lpt0_found = true;
            break;
        }
    }
    for (int i = 0; i < MIN(far_rpts1s_num, 80); i++) {
        if (fabs(far_rpts1an[i])<0.0001) continue;
        int im1 = clip(i - (int) round(angle_dist / sample_dist), 0, far_rpts1s_num - 1);
        int ip1 = clip(i + (int) round(angle_dist / sample_dist), 0, far_rpts1s_num - 1);
        float conf = fabs(far_rpts1a[i]) - (fabs(far_rpts1a[im1]) + fabs(far_rpts1a[ip1])) / 2;

        if (45. / 180. * PI < conf && conf < 110. / 180. * PI && i < 100) {
            far_Lpt1_rpts1s_id = i;
            far_Lpt1_found = true;
            break;
        }
    }
}
