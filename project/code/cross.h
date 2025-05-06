#ifndef __CROSS_H__
#define	__CROSS_H__
#include "Image_process.h"
#define FAR_POINTS_MAX_LEN  (POINTS_MAX_LEN)
enum cross_type_e {
    CROSS_NONE = 0,     //
    CROSS_BEGIN,        // 
    CROSS_IN,           // 
    CROSS_NUM,
};

enum track_type_e {
    TRACK_LEFT=0,
    TRACK_RIGHT,
		TRACK_MIX
};

extern enum cross_type_e cross_type;

extern enum track_type_e track_type;

extern const char *cross_type_name[CROSS_NUM];


void check_cross();

void run_cross();

void draw_cross();

void cross_farline();
#endif