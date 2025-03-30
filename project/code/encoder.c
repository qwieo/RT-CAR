#include "encoder.h"

int16 M1,M2,M3;

void encoder_init(void)
{
	encoder_dir_init(QTIMER2_ENCODER1,QTIMER2_ENCODER1_CH1_C3,QTIMER2_ENCODER1_CH2_C25);//M3
	encoder_dir_init(QTIMER1_ENCODER2,QTIMER1_ENCODER2_CH1_C2,QTIMER1_ENCODER2_CH2_C24);//M2
	encoder_dir_init(QTIMER3_ENCODER2,QTIMER3_ENCODER2_CH1_B18,QTIMER3_ENCODER2_CH2_B19);//M1
}
int16 get_encoder(void)
{
	M1=-encoder_get_count(QTIMER3_ENCODER2);
	encoder_clear_count(QTIMER3_ENCODER2);
	M2=-encoder_get_count(QTIMER1_ENCODER2);
	encoder_clear_count(QTIMER1_ENCODER2);
	M3=-encoder_get_count(QTIMER2_ENCODER1);
	encoder_clear_count(QTIMER2_ENCODER1);	
	return M2;
}
