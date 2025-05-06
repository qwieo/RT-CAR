#include "zf_common_headfile.h"
#define RESULT_ROW 100
#define RESULT_COL 114
#define         USED_ROW                120  
#define         USED_COL                188
#define PER_IMG     mt9v03x_image
#define ImageUsed   *PerImg_ip                                          
uint8_t *PerImg_ip[RESULT_ROW][RESULT_COL];
uint8_t LeftColor = 0; 
uint8_t RightColor =0; 
uint8_t WhiteColor =255; 
void ImagePerspective_Init(void) {
		/*the only part can change through upper computer*/
   double change_un_Mat[3][3] ={{0.209024,-0.166388,10.855739},{-0.003405,0.013697,4.739457},{0.000026,-0.001729,0.237276}};
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
            if (local_x>= 0&& local_y >= 0 && local_y < USED_ROW && local_x < USED_COL){
                PerImg_ip[j][i] = &PER_IMG[local_y][local_x];
            }
            else {
							if(i<57){
								if(j>84)
                PerImg_ip[j][i] = &LeftColor;
								else
								PerImg_ip[j][i] = &WhiteColor;	
							}
							else{
								if(j>84)								
								PerImg_ip[j][i] = &RightColor;
								else
								PerImg_ip[j][i] = &WhiteColor;	
							}								
							
            }
		}

    }
 
}
