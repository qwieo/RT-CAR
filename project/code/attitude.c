#include "zf_common_headfile.h"
extern int16 imu963ra_acc_x,  imu963ra_acc_y,  imu963ra_acc_z;
extern int16 imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z;
extern int16 imu963ra_mag_x,  imu963ra_mag_y,  imu963ra_mag_z;
extern float imu963ra_transition_factor[3];
void    imu963ra_get_acc            (void);
void    imu963ra_get_gyro           (void);
void    imu963ra_get_mag            (void);
#define imu963ra_acc_transition(acc_value)      ((float)(acc_value) / imu963ra_transition_factor[0])
#define imu963ra_gyro_transition(gyro_value)    ((float)(gyro_value) / imu963ra_transition_factor[1])
#define imu963ra_mag_transition(mag_value)    ((float)(mag_value) / imu963ra_transition_factor[2])
	

