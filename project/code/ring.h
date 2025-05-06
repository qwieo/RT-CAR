#ifndef RING_H
#define RING_H
#include "zf_common_headfile.h"
#include "cross.h"
enum circle_type_e {
    CIRCLE_NONE = 0,                            
    CIRCLE_LEFT_BEGIN, CIRCLE_RIGHT_BEGIN,      
    CIRCLE_LEFT_IN, CIRCLE_RIGHT_IN,            
    CIRCLE_LEFT_RUNNING, CIRCLE_RIGHT_RUNNING, 
    CIRCLE_LEFT_OUT, CIRCLE_RIGHT_OUT,          
    CIRCLE_LEFT_END, CIRCLE_RIGHT_END,          
    CIRCLE_NUM,                                
};

extern enum circle_type_e circle_type;
extern float sample_dist;
extern float begin_y;
void check_circle();

void run_circle();

void draw_circle();

#endif 