#include "Image_process.h"

/*
*@Name          :clip
*@Description   :limit x value betwen low and up
*@Param         :
*@Return        :int
*@Sample        :clip(x,low,up);
*/
int clip(int x, int low, int up) {
    return x > up ? up : x < low ? low : x;
}


#if 1
AT_ITCM_SECTION_INIT(void clear_image(image_t *img)) {
    assert(img && img->data);
    if (img->width == img->step) {
        memset(img->data, 0, img->width * img->height);
    } else {
        for (int y = 0; y < img->height; y++) {
            memset(&AT(img, 0, y), 0, img->width);
        }
    }
}

AT_ITCM_SECTION_INIT(void clone_image(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    if (img0->width == img0->step && img1->width == img1->step) {
        memcpy(img1->data, img0->data, img0->width * img0->height);
    } else {
        for (int y = 0; y < img0->height; y++) {
            memcpy(&AT(img1, 0, y), &AT(img0, 0, y), img0->width);
        }
    }
}
/*
*@Name          :adaptive_threshold
*@Description   :adaptive_threshold and turn to binary
*@Param         :
*@Return        :void
*@Sample        :adaptive_threshold(&img0,&img1,5,2,0,255);
*/
AT_ITCM_SECTION_INIT(void adaptive_threshold(image_t *img0, image_t *img1, int block_size, int down_value, uint8_t low_value, uint8_t high_value)){
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->data != img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(block_size > 1 && block_size % 2 == 1);

    int half = block_size / 2;
    for (int y = 0; y < img0->height; y++) {
        for (int x = 0; x < img0->width; x++) {
            int thres_value = 0;
            for (int dy = -half; dy <= half; dy++) {
                for (int dx = -half; dx <= half; dx++) {
                    thres_value += AT_CLIP(img0, x + dx, y + dy);
                }
            }
            thres_value /= block_size * block_size;
            thres_value -= down_value;
                    AT(img1, x, y) = AT(img0, x, y) < thres_value ? low_value : high_value;
        }
    }
}
#endif
#if 1


void draw_x(image_t *img, int x, int y, int len, uint8_t value) {
    for (int i = -len; i <= len; i++) {
                AT(img, clip(x + i, 0, img->width - 1), clip(y + i, 0, img->height - 1)) = value;
                AT(img, clip(x - i, 0, img->width - 1), clip(y + i, 0, img->height - 1)) = value;
    }
}


void draw_o(image_t *img, int x, int y, int radius, uint8_t value) {
    for (float i = -PI; i <= PI; i += PI / 10) {
                AT(img, clip(x + radius * cosf(i), 0, img->width - 1), clip(y + radius * sinf(i), 0, img->height - 1)) = value;
    }
}


void draw(void)
{
//	for(uint8 i=1;i<rpts0s_num;i++)
//	show0[clip(rptsc0[rpts0s_num-i][1],0,99)][clip(rptsc0[rpts0s_num-i][0],0,113)]=0;
//	draw_o(&img_raw,clip(rptsc0[rpts0s_num][0],0,113),clip(rptsc0[rpts0s_num][1],0,99),4,0);
//	for(uint8_t i=1;i<rpts1s_num;i++)
//	show0[clip(rptsc1[rpts1s_num-i][1],0,99)][clip(rptsc1[rpts1s_num-i][0],0,113)]=0;
//	draw_o(&img_raw,clip(rptsc1[rpts1s_num][0],0,113),clip(rptsc1[rpts1s_num][1],0,99),4,0);
	
	for(uint8 i=1;i<rpts0s_num;i++)
	ips200_draw_point(clip(rpts0s[rpts0s_num-i][0],0,99),clip(rpts0s[rpts0s_num-i][1],0,113),RGB565_GREEN);
	draw_o(&img_raw,clip(rpts0s[rpts0s_num][0],0,113),clip(rpts0s[rpts0s_num][1],0,99),4,0);
	for(uint8_t i=1;i<rpts1s_num;i++)
	ips200_draw_point(clip(rpts1s[rpts1s_num-i][0],0,99),clip(rpts1s[rpts1s_num-i][1],0,113),RGB565_GREEN);
	draw_o(&img_raw,clip(rpts1s[rpts1s_num][0],0,113),clip(rpts1s[rpts1s_num][1],0,99),4,0);
	if(Lpt0_found)
	{
		draw_x(&img_raw,clip(rpts0s[Lpt0_rpts0s_id][0],0,113),clip(rpts0s[Lpt0_rpts0s_id][1],0,99),6,0);
	}
	if(Lpt1_found)
	{
		draw_x(&img_raw,clip(rpts1s[Lpt1_rpts1s_id][0],0,113),clip(rpts1s[Lpt1_rpts1s_id][1],0,99),6,0);
	}
	
	
}

void IPMcopy()
{
                for(int i=0;i<RESULT_ROW;i++)
                {
                    for(int j=0;j<RESULT_COL;j++)
                    {									
												show0[i][j]=*ImageUsed[i][j];																				
                    }
                }
}

#endif
#if 1
AT_ITCM_SECTION_INIT(void dilate3(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    int max_value;  
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
            max_value = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (AT(img0, x + dx, y + dy) > max_value) max_value = AT(img0, x + dx, y + dy);
                }
            }
                    AT(img1, x, y) = max_value;
        }
    }
}

AT_ITCM_SECTION_INIT(void erode3(image_t *img0, image_t *img1)) {
    assert(img0 && img0->data);
    assert(img1 && img1->data);
    assert(img0->width == img1->width && img0->height == img1->height);
    assert(img0 != img1 && img0->data != img1->data);

    int min_value;
    for (int y = 1; y < img0->height - 1; y++) {
        for (int x = 1; x < img0->width - 1; x++) {
            min_value = 255;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (AT(img0, x + dx, y + dy) < min_value) min_value = AT(img0, x + dx, y + dy);
                }
            }
                    AT(img1, x, y) = min_value;
        }
    }
}

#endif
/* 
 *   0
 * 3   1
 *   2
 */
AT_DTCM_SECTION_ALIGN_INIT(const int dir_front[4][2], 8) = {{0,  -1},
                                                            {1,  0},
                                                            {0,  1},
                                                            {-1, 0}};
AT_DTCM_SECTION_ALIGN_INIT(const int dir_frontleft[4][2], 8) = {{-1, -1},
                                                                {1,  -1},
                                                                {1,  1},
                                                                {-1, 1}};
AT_DTCM_SECTION_ALIGN_INIT(const int dir_frontright[4][2], 8) = {{1,  -1},
                                                                 {1,  1},
                                                                 {-1, 1},
                                                                 {-1, -1}};


AT_ITCM_SECTION_INIT(void findline_lefthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)) {
    assert(img && img->data);
    assert(num && *num >= 0);
    assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && half < x && x < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4) {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;

        int current_value = AT(img, x, y);
        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontleft_value = AT(img, x + dir_frontleft[dir][0], y + dir_frontleft[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 1) % 4;
            turn++;
        } else if (frontleft_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontleft[dir][0];
            y += dir_frontleft[dir][1];
            dir = (dir + 3) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}


AT_ITCM_SECTION_INIT(void findline_righthand_adaptive(image_t *img, int block_size, int clip_value, int x, int y, int pts[][2], int *num)) {
    assert(img && img->data);
    assert(num && *num >= 0);
    assert(block_size > 1 && block_size % 2 == 1);
    int half = block_size / 2;
    int step = 0, dir = 0, turn = 0;
    while (step < *num && half < x && x < img->width - half - 1 && half < y && y < img->height - half - 1 && turn < 4) {
        int local_thres = 0;
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                local_thres += AT(img, x + dx, y + dy);
            }
        }
        local_thres /= block_size * block_size;
        local_thres -= clip_value;

        int current_value = AT(img, x, y);
        int front_value = AT(img, x + dir_front[dir][0], y + dir_front[dir][1]);
        int frontright_value = AT(img, x + dir_frontright[dir][0], y + dir_frontright[dir][1]);
        if (front_value < local_thres) {
            dir = (dir + 3) % 4;
            turn++;
        } else if (frontright_value < local_thres) {
            x += dir_front[dir][0];
            y += dir_front[dir][1];
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        } else {
            x += dir_frontright[dir][0];
            y += dir_frontright[dir][1];
            dir = (dir + 1) % 4;
            pts[step][0] = x;
            pts[step][1] = y;
            step++;
            turn = 0;
        }
    }
    *num = step;
}

AT_ITCM_SECTION_INIT(void blur_points(int pts_in[][2], int num, float pts_out[][2], int kernel)) {
    assert(kernel % 2 == 1);
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++) {
            pts_out[i][0] += pts_in[clip(i + j, 0, num - 1)][0] * (half + 1 - abs(j));
            pts_out[i][1] += pts_in[clip(i + j, 0, num - 1)][1] * (half + 1 - abs(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}

AT_ITCM_SECTION_INIT(void resample_points(float pts_in[][2], int num1, float pts_out[][2], int *num2, float dist)){
    int remain = 0, len = 0;
    for(int i=0; i<num1-1 && len < *num2; i++){
        float x0 = pts_in[i][0];
        float y0 = pts_in[i][1];
        float dx = pts_in[i+1][0] - x0;
        float dy = pts_in[i+1][1] - y0;
        float dn = sqrt(dx*dx+dy*dy);
        dx /= dn;
        dy /= dn;

        while(remain < dn && len < *num2){
            x0 += dx * remain;
            pts_out[len][0] = x0;
            y0 += dy * remain;
            pts_out[len][1] = y0;
            
            len++;
            dn -= remain;
            remain = dist;
        }
        remain -= dn;
    }
    *num2 = len;
}
AT_ITCM_SECTION_INIT(void track_leftline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist)) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] - dy * dist;
        pts_out[i][1] = pts_in[i][1] + dx * dist;
    }
}

AT_ITCM_SECTION_INIT(void track_rightline(float pts_in[][2], int num, float pts_out[][2], int approx_num, float dist)) {
    for (int i = 0; i < num; i++) {
        float dx = pts_in[clip(i + approx_num, 0, num - 1)][0] - pts_in[clip(i - approx_num, 0, num - 1)][0];
        float dy = pts_in[clip(i + approx_num, 0, num - 1)][1] - pts_in[clip(i - approx_num, 0, num - 1)][1];
        float dn = sqrt(dx * dx + dy * dy);
        dx /= dn;
        dy /= dn;
        pts_out[i][0] = pts_in[i][0] + dy * dist;
        pts_out[i][1] = pts_in[i][1] - dx * dist;
    }
}

AT_ITCM_SECTION_INIT(void local_angle_points(float pts_in[][2], int num, float angle_out[], int dist)) {
    for (int i = 0; i < num; i++) {
        if (i <= 0 || i >= num - 1) {
            angle_out[i] = 0;
            continue;
        }
        float dx1 = pts_in[i][0] - pts_in[clip(i - dist, 0, num - 1)][0];
        float dy1 = pts_in[i][1] - pts_in[clip(i - dist, 0, num - 1)][1];
        float dn1 = sqrtf(dx1 * dx1 + dy1 * dy1);
        float dx2 = pts_in[clip(i + dist, 0, num - 1)][0] - pts_in[i][0];
        float dy2 = pts_in[clip(i + dist, 0, num - 1)][1] - pts_in[i][1];
        float dn2 = sqrtf(dx2 * dx2 + dy2 * dy2);
        float c1 = dx1 / dn1;
        float s1 = dy1 / dn1;
        float c2 = dx2 / dn2;
        float s2 = dy2 / dn2;
        angle_out[i] = atan2f(c1 * s2 - c2 * s1, c2 * c1 + s2 * s1);
    }
}

AT_ITCM_SECTION_INIT(void nms_angle(float angle_in[], int num, float angle_out[], int kernel)) {
    assert(kernel % 2 == 1);
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        angle_out[i] = angle_in[i];
        for (int j = -half; j <= half; j++) {
            if (fabs(angle_in[clip(i + j, 0, num - 1)]) > fabs(angle_out[i])) {
                angle_out[i] = 0;
                break;
            }
        }
    }
}
