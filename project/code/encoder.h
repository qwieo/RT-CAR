#include "zf_common_headfile.h"
#ifndef __ENCODER__H__
#define __ENCODER__H__
#define ENCODER_PER_METER   (21200)

void encoder_init(void);
int16 get_encoder(void);
#endif 
