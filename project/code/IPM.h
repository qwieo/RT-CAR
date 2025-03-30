#ifndef __IPM_H__
#define	__IPM_H__
#include "zf_common_headfile.h"
#define RESULT_ROW 100
#define RESULT_COL 114
#define         USED_ROW                120  
#define         USED_COL                188
#define PER_IMG     mt9v03x_image
#define ImageUsed   PerImg_ip     
extern uint8_t *PerImg_ip[RESULT_ROW][RESULT_COL];
void ImagePerspective_Init(void);

#endif