#include "zf_common_headfile.h"
#define RESULT_ROW 100
#define RESULT_COL 114
#define         USED_ROW                120  
#define         USED_COL                188
#define PER_IMG     mt9v03x_image
#define ImageUsed   *PerImg_ip                                          
uint8_t *PerImg_ip[RESULT_ROW][RESULT_COL];
 
void ImagePerspective_Init(void) {
 
    static uint8_t BlackColor = 0;
		/*the only part can change through upper computer*/
    double change_un_Mat[3][3] =
		{{-0.061265,0.056376,-3.917669},
		{-0.000000,0.001279,-2.511940},
		{-0.000000,0.000547,-0.073652}};
		/*************************************************/
    for (int i = 0; i < RESULT_COL ;i++) {
        for (int j = 0; j < RESULT_ROW ;j++) {
            int local_x = (int) ((change_un_Mat[0][0] * i
                    + change_un_Mat[0][1] * j + change_un_Mat[0][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            int local_y = (int) ((change_un_Mat[1][0] * i
                    + change_un_Mat[1][1] * j + change_un_Mat[1][2])
                    / (change_un_Mat[2][0] * i + change_un_Mat[2][1] * j
                            + change_un_Mat[2][2]));
            if (local_x
                    >= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL){
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else {
                PerImg_ip[j][i] = &BlackColor;          //&PER_IMG[0][0];
            }
					}

    }
 
}
