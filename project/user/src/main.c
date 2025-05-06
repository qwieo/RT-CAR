/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
#include "para_space.h"
// 本例程是开源库移植用空工程
/*      每日一拜 好过省赛             天天参拜 稳过国赛        */
/* \\ \\ \\ \\ \\ \\ \\ || || || || || || // // // // // // // //
\\ \\ \\ \\ \\ \\ \\        _ooOoo_          // // // // // // //
\\ \\ \\ \\ \\ \\          o8888888o            // // // // // //
\\ \\ \\ \\ \\             88" . "88               // // // // //
\\ \\ \\ \\                (| -_- |)                  // // // //
\\ \\ \\                   O\  =  /O                     // // //
\\ \\                   ____/`---'\____                     // //
\\                    .'  \\|     |//  `.                      //
==                   /  \\|||  :  |||//  \                     ==
==                  /  _||||| -:- |||||-  \                    ==
==                  |   | \\\  -  /// |   |                    ==
==                  | \_|  ''\---/''  |   |                    ==
==                  \  .-\__  `-`  ___/-. /                    ==
==                ___`. .'  /--.--\  `. . ___                  ==
==              ."" '<  `.___\_<|>_/___.'  >'"".               ==
==            | | :  `- \`.;`\ _ /`;.`/ - ` : | |              \\
//            \  \ `-.   \_ __\ /__ _/   .-` /  /              \\
//      ========`-.____`-.___\_____/___.-`____.-'========      \\
//                           `=---='                           \\
// //   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  \\ \\
// // //      佛祖保佑      永无BUG      永不修改          \\ \\ \\
// // // // // // || || || || || || || || || || \\ \\ \\ \\ \\ */
float ta,L2_gyro_z,L_gyro_z,new_gyro_z;
uint8 art_flag,art1_flag;
float dangle,langle,w;
//
TaskHandle_t				AppTaskCreate_Handle = NULL;

TaskHandle_t 			 			 IPS_Task_Handle = NULL;
TaskHandle_t 		Process_image_Task_Handle= NULL;
TaskHandle_t 	 	 Find_corners_Task_Handle= NULL;
TaskHandle_t  System_identify_Task_Handle= NULL;
TaskHandle_t 						Brake_Task_Handle= NULL;
TaskHandle_t 						  Key_Task_Handle= NULL;
TaskHandle_t 				  Element_Task_Handle= NULL;
TaskHandle_t 						Alloc_Task_Handle= NULL;
TaskHandle_t 						Judge_Task_Handle= NULL;
TaskHandle_t 						 PUSH_Task_Handle= NULL;
TaskHandle_t 						 Test_Task_Handle= NULL;

void 							ipsTask(void* parameter);
void  process_image_Task(void * parameter);
void    find_corners_Task(void* parameter);
void System_Identify_Task(void* parameter);
void 		   		 Brake_Task(void* parameter);
void 			 	 	   Key_Task(void* parameter);
void 		 		 Element_Task(void* parameter);
void 		   		 Alloc_Task(void* parameter);
void 		   		 Judge_Task(void* parameter);
void 						PUSH_Task(void* parameter);
void						Test_Task(void* parameter);
void AppTaskCreate(void);
void PrintTaskList(void);
void PrintRunTimeStats(void);

SemaphoreHandle_t BinarySem0_Handle =NULL;
SemaphoreHandle_t BinarySem1_Handle =NULL;
SemaphoreHandle_t BinarySem2_Handle =NULL;
SemaphoreHandle_t BinarySem3_Handle =NULL;
SemaphoreHandle_t BinarySem4_Handle =NULL;
SemaphoreHandle_t BinarySem5_Handle =NULL;
int main(void){
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                  // 调试端口初始化
    // 此处编写用户代码 例如外设初始化代码等

	wireless_uart_init();
	system_delay_ms(300);
//		//my_spi_wifi_init();
  judge_init();
	interrupt_set_priority(PIT_IRQn, 0);
	interrupt_global_enable(0);
	
	gpio_init(B11,GPO,0,GPO_PUSH_PULL);
	
	imu963ra_init ();
	encoder_init();
	pit_ms_init(PIT_CH3,5);
			
	ips200_set_dir(IPS200_CROSSWISE_180);
	ips200_init(IPS200_TYPE_SPI);

	key_init(10);    
	Motor_Init();	
	while(mt9v03x_init()){
	printf("camera Init error");}
	
	
//		my_spi_wifi_image_init();
	while(art_flag!=255||art1_flag!=255)
	{;}
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate, 
                        (const char*    )"AppTaskCreate",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )1, 
                        (TaskHandle_t*  )&AppTaskCreate_Handle);
          
	if(pdPASS == xReturn)
		vTaskStartScheduler();   
	else
		return -1;  
				
    while(1)
    {
		;
    }
	}
void AppTaskCreate(void){
  BaseType_t xReturn = pdPASS;  
  taskENTER_CRITICAL();           
	BinarySem0_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem0_Handle)
    printf("BinarySem0_Handle Create success\r\n");
	
	BinarySem1_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem1_Handle)
    printf("BinarySem1_Handle Create success\r\n");
	BinarySem2_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem2_Handle)
    printf("BinarySem2_Handle Create success\r\n");
	BinarySem3_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem3_Handle)
    printf("BinarySem3_Handle Create success\r\n");
	BinarySem4_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem4_Handle)
    printf("BinarySem4_Handle Create success\r\n");
		BinarySem5_Handle = xSemaphoreCreateBinary();    
  if(NULL != BinarySem5_Handle)
    printf("BinarySem5_Handle Create success\r\n");

  xReturn = xTaskCreate((TaskFunction_t )ipsTask,  
                        (const char*    )"IPS_Task",
                        (uint16_t       )320,  
                        (void*          )NULL,
                        (UBaseType_t    )3, 
                        (TaskHandle_t*  )&IPS_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建IPS_Task任务成功!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )System_Identify_Task,  
                        (const char*    )"System_identify_Task",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )1, 
                        (TaskHandle_t*  )&System_identify_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建SI任务成功!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )process_image_Task,  
                        (const char*    )"process_image_Task",
                        (uint16_t       )192,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Process_image_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建process_image任务成功!\r\n");
	xReturn = xTaskCreate((TaskFunction_t )Brake_Task,  
                        (const char*    )"Brake_Task",
                        (uint16_t       )384,  
                        (void*          )NULL,
                        (UBaseType_t    )1, 
                        (TaskHandle_t*  )&Brake_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建Brake任务成功!\r\n");
	xReturn = xTaskCreate((TaskFunction_t )Key_Task,  
                        (const char*    )"Key_Task",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )3, 
                        (TaskHandle_t*  )&Key_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建Key任务成功!\r\n");

	xReturn = xTaskCreate((TaskFunction_t )find_corners_Task,  
                        (const char*    )"Find_corners_Task",
                        (uint16_t       )192,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Find_corners_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建Find_corners任务成功!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )Element_Task,  
                        (const char*    )"Element_Task",
                        (uint16_t       )384,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Element_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建Element任务成功!\r\n");
	
	
	
		
//	xReturn = xTaskCreate((TaskFunction_t )Judge_Task,  
//                        (const char*    )"Judge_Task",
//                        (uint16_t       )128,  
//                        (void*          )NULL,
//                        (UBaseType_t    )2, 
//                        (TaskHandle_t*  )&Judge_Task_Handle);
//  if(pdPASS == xReturn)
//    printf("创建Judge任务成功!\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t )PUSH_Task,  
                        (const char*    )"PUSH_Task",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )3, 
                        (TaskHandle_t*  )&PUSH_Task_Handle);
  if(pdPASS == xReturn)
    printf("创建PUSH任务成功!\r\n");
	
//		xReturn = xTaskCreate((TaskFunction_t )Test_Task,  
//                        (const char*    )"Test_Task",
//                        (uint16_t       )256,  
//                        (void*          )NULL,
//                        (UBaseType_t    )2, 
//                        (TaskHandle_t*  )&Test_Task_Handle);
//  if(pdPASS == xReturn)
//    printf("创建Test任务成功!\r\n");

//	xReturn = xTaskCreate((TaskFunction_t )Alloc_Task,  
//                        (const char*    )"Alloc_Task",
//                        (uint16_t       )256,  
//                        (void*          )NULL,
//                        (UBaseType_t    )3, 
//                        (TaskHandle_t*  )&Alloc_Task_Handle);
//  if(pdPASS == xReturn)
//    printf("创建Alloc任务成功!\r\n");
	
  vTaskDelete(AppTaskCreate_Handle); 
  
  taskEXIT_CRITICAL();            
}
void ipsTask(void* parameter){
	while(1)
	{	
//		debug_draw();			
//		ips200_show_gray_image(0,0,show0[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
		ips200_show_uint(0,192,circle_type,2);
		ips200_show_uint(0,208,Pestate,2);
		ips200_show_uint(0,160,aw,1);
//		ips200_show_float(0,192,cps*180./PI,3,2);
//		ips200_show_uint(0,192,judge_uart_buffer[6],3);
			ips200_show_int(0,128,x_error,4);
		
		vTaskDelay(5);
	}
}
void System_Identify_Task(void* parameter){
	int i=0;
	uint8 s=0,ls=0;
	BaseType_t xReturn = pdPASS;
	vTaskSuspend(System_identify_Task_Handle);
	while(1)
	{
		xReturn = xSemaphoreTake(BinarySem2_Handle,
                              portMAX_DELAY);
					preanchor_strategy(v);
					a0=calculator(aim0),a1=calculator(aim1);
					a=(a0+a1)/2;
					if(a>=0)
						s=1;
					else
						s=0;
					if(s-ls!=0)
						Gangle.out_i=0;						
						w=dangle*500.f;
					
					if(track_type == TRACK_MIX){
						speed_strategy(a);
//						lyft_basic( 0,PidLocCtrl(&SY,st,0),3*func_limit_ab(PidLocCtrl(&Gangle,dangle,a),-16.f,16.f));
						lyft_basic( -v,0,3*func_limit_ab(PidLocCtrl(&Gangle,dangle,a),-16.f,16.f));
					}
						
					else if(track_type == TRACK_LEFT)
						{	speed_strategy(a0);
							lyft_basic( -v,0,3*func_limit_ab(PidLocCtrl(&Langle,dangle,a0),-16.f,16.f));
						}
					else if(track_type == TRACK_RIGHT)
						{	speed_strategy(a1);
							lyft_basic( -v,0,3*func_limit_ab(PidLocCtrl(&Rangle,dangle,a1),-16.f,16.f));
						}
					ls=s;
	vTaskDelay(5);
		
	}
}
void Brake_Task(void* parameter){
	uint16 cnt=0,cnt1=0;
	bool status;
	BaseType_t xReturn = pdPASS;
	
	while(1)
	{
		xReturn = xSemaphoreTake(BinarySem0_Handle,
                              portMAX_DELAY);
		
		status=check_zebra();
		if((ipts0_num==0&&ipts1_num==0)||status)
			{
				cnt++;
				if(cnt>10)
				{
					lyft_basic(0,0,0);
					vTaskSuspend(System_identify_Task_Handle);
				}
				else if(status)
				{
					vTaskDelay(300);
					Motor_Init();
					vTaskSuspend(System_identify_Task_Handle);
				}
			}
			else
			{
			cnt=0;
			}	
		vTaskDelay(5);
	}
}	
void Key_Task(void* parameter){
	while(1){
		key_scanner();
		if(key_get_state(KEY_1)==KEY_SHORT_PRESS){
			PID_Init(&m1);
			PID_Init(&m2);
			PID_Init(&m3);
			PID_Init(&Gangle);
			vTaskResume(System_identify_Task_Handle);}
		if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
			vTaskResume(Judge_Task_Handle);
		vTaskDelay(10);}
}
void process_image_Task(void * parameter) {
	ImagePerspective_Init();
	BaseType_t xReturn = pdPASS;
	while(1)
	{
		if(mt9v03x_finish_flag){
			mt9v03x_finish_flag=0;
			IPMcopy();
			blur(&img_raw,&img_AP,5);
			clone_image(&img_AP,&img_raw);
			
			int x1 = img_raw.width / 2 - begin_x, y1 = begin_y;
			ipts0_num = sizeof(ipts0) / sizeof(ipts0[0]);
			for (; x1 > 0; x1--) if (AT_IMAGE(&img_raw, x1 - 1, y1) < thres) break;
			if (AT_IMAGE(&img_raw, x1, y1) >= thres)
			findline_lefthand_adaptive(&img_raw, block_size, clip_value, x1, y1, ipts0, &ipts0_num);
			else 
				ipts0_num=0;
	
			int x2 = img_raw.width / 2 + begin_x, y2 = begin_y;
			ipts1_num = sizeof(ipts1) / sizeof(ipts1[0]);
			for (; x2 < img_raw.width - 1; x2++) if (AT_IMAGE(&img_raw, x2 + 1, y2) < thres) break;
			if (AT_IMAGE(&img_raw, x2, y2) >= thres)
				findline_righthand_adaptive(&img_raw, block_size, clip_value, x2, y2, ipts1, &ipts1_num);
			else 
				ipts1_num=0;
		
			blur_points(ipts0, ipts0_num, rpts0b, (int) round(line_blur_kernel));
			rpts0b_num = ipts0_num;
			blur_points(ipts1, ipts1_num, rpts1b, (int) round(line_blur_kernel));
			rpts1b_num = ipts1_num;
		
			rpts0s_num = sizeof(rpts0s) / sizeof(rpts0s[0]);
			resample_points(rpts0b, rpts0b_num, rpts0s, &rpts0s_num, sample_dist * pixel_per_meter);
			rpts1s_num = sizeof(rpts1s) / sizeof(rpts1s[0]);
			resample_points(rpts1b, rpts1b_num, rpts1s, &rpts1s_num, sample_dist * pixel_per_meter);
		
		
		
			local_angle_points(rpts0s, rpts0s_num, rpts0a, (int) round(angle_dist / sample_dist));
			rpts0a_num = rpts0s_num;
			local_angle_points(rpts1s, rpts1s_num, rpts1a, (int) round(angle_dist / sample_dist));
			rpts1a_num = rpts1s_num;

    
			nms_angle(rpts0a, rpts0a_num, rpts0an, (int) round(angle_dist / sample_dist) * 2 + 1);
			rpts0an_num = rpts0a_num;
			nms_angle(rpts1a, rpts1a_num, rpts1an, (int) round(angle_dist / sample_dist) * 2 + 1);
			rpts1an_num = rpts1a_num;
		
		
			track_leftline(rpts0s, rpts0s_num, rptsc0, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
			rptsc0_num = rpts0s_num;
			track_rightline(rpts1s, rpts1s_num, rptsc1, (int) round(angle_dist / sample_dist), pixel_per_meter * ROAD_WIDTH / 2);
			rptsc1_num = rpts1s_num;
						
			rptscs0_num = 100;
			resample_points(rptsc0, rptsc0_num, rptscs0, &rptscs0_num, sample_dist * pixel_per_meter);
			rptscs1_num = 100;
			resample_points(rptsc1, rptsc1_num, rptscs1, &rptscs1_num, sample_dist * pixel_per_meter);		
		}
			xReturn = xSemaphoreGive( BinarySem0_Handle );
			vTaskDelay(5);
		}
		
}
void find_corners_Task(void* parameter) {
	BaseType_t xReturn = pdPASS;
	while(1)
	{
		Lpt0_found = Lpt1_found = false;
    is_straight0 = rpts0s_num > 1. / sample_dist;
    is_straight1 = rpts1s_num > 1. / sample_dist;
    for (int i = 0; i < rpts0s_num; i++) {
        if (rpts0an[i] == 0) continue;
        int im1 = clip(i - (int) round(angle_dist / sample_dist), 0, rpts0s_num - 1);
        int ip1 = clip(i + (int) round(angle_dist / sample_dist), 0, rpts0s_num - 1);
        float conf = fabs(rpts0a[i]) - (fabs(rpts0a[im1]) + fabs(rpts0a[ip1])) / 2;

        //L-corner
        if (Lpt0_found == false && 70. / 180. * PI < conf && conf < PI && i < 0.8 / sample_dist) {
            Lpt0_rpts0s_id = i;
            Lpt0_found = true;
        }
        //straight road 
        if (conf > 3. / 180. * PI && i < 1. / sample_dist) is_straight0 = false;
        if (Lpt0_found == true && is_straight0 == false) break;
    }
    for (int i = 0; i < rpts1s_num; i++) {
        if (rpts1an[i] == 0) continue;
        int im1 = clip(i - (int) round(angle_dist / sample_dist), 0, rpts1s_num - 1);
        int ip1 = clip(i + (int) round(angle_dist / sample_dist), 0, rpts1s_num - 1);
        float conf = fabs(rpts1a[i]) - (fabs(rpts1a[im1]) + fabs(rpts1a[ip1])) / 2;
       
        if (Lpt1_found == false && 70. / 180. * PI < conf && conf < PI && i < 0.8 / sample_dist) {
            Lpt1_rpts1s_id = i;
            Lpt1_found = true;
        }

        if (conf > 3. / 180. * PI && i < 1. / sample_dist) is_straight1 = false;

        if (Lpt1_found == true && is_straight1 == false) break;
    }


        if (Lpt0_found && Lpt1_found) {
            float dx = rpts0s[Lpt0_rpts0s_id][0] - rpts1s[Lpt1_rpts1s_id][0];
            float dy = rpts0s[Lpt0_rpts0s_id][1] - rpts1s[Lpt1_rpts1s_id][1];
            float dn = sqrtf(dx * dx + dy * dy);
            if (fabs(dn - 0.45 * pixel_per_meter) < 0.15 * pixel_per_meter) {
                float dwx = rpts0s[clip(Lpt0_rpts0s_id + 50, 0, rpts0s_num - 1)][0] -
                            rpts1s[clip(Lpt1_rpts1s_id + 50, 0, rpts1s_num - 1)][0];
                float dwy = rpts0s[clip(Lpt0_rpts0s_id + 50, 0, rpts0s_num - 1)][1] -
                            rpts1s[clip(Lpt1_rpts1s_id + 50, 0, rpts1s_num - 1)][1];
                float dwn = sqrtf(dwx * dwx + dwy * dwy);
                if (!(dwn > 0.7 * pixel_per_meter &&
                      rpts0s[clip(Lpt0_rpts0s_id + 50, 0, rpts0s_num - 1)][0] < rpts0s[Lpt0_rpts0s_id][0] &&
                      rpts1s[clip(Lpt1_rpts1s_id + 50, 0, rpts1s_num - 1)][0] > rpts1s[Lpt1_rpts1s_id][0])) {
                    Lpt0_found = Lpt1_found = false;
                }
            } else {
                Lpt0_found = Lpt1_found = false;
            }
        }
    
		xReturn = xSemaphoreGive( BinarySem1_Handle );
		vTaskDelay(5);
	}
}
void Element_Task(void* parameter){
	BaseType_t xReturn = pdPASS;
	while(1){
		xReturn = xSemaphoreTake(BinarySem1_Handle,portMAX_DELAY);	
		check_circle();
		run_circle();
		ips200_show_uint(0,195,circle_type,3);
		vTaskDelay(5);}	
}
void Alloc_Task(void* parameter){
	while(1){
		PrintRunTimeStats();				
		PrintTaskList();
		vTaskDelay(2000);}
}
void Judge_Task(void* parameter){
	uint16 x=0,y=0;
	uint8 index=1;
	vTaskSuspend(Judge_Task_Handle);
	while(1){
		for(uint8 i=0;i<2;i++){
			ips200_show_uint(x,y,index,2);
			x+=32;
			ips200_show_string(x,y,".");
			x+=32;
			x=0;
			y+=32;
			index++;}
		vTaskSuspend(Judge_Task_Handle);}
}
void PUSH_Task(void* parameter){
	BaseType_t xReturn = pdPASS;
	bool isuspend=false;
	int16 push_encoder=0;
	uint8 ri=0;
	while(1)
	{
		xReturn = xSemaphoreTake(BinarySem4_Handle,portMAX_DELAY);
		switch (Pestate)
		{
			case PUSH_IDLE:
			xReturn = xSemaphoreTake(BinarySem3_Handle,portMAX_DELAY);
			if(!isuspend){
				vTaskSuspend(System_identify_Task_Handle);
				vTaskSuspend(Brake_Task_Handle);
				vTaskSuspend(Element_Task_Handle);
				box_check();
				cps=clip(angle_compaste(),-PI/2,PI/2);
				isuspend=true;}
			anglez=0;
			Pestate=PUSH_ROLL;
			case PUSH_RUN:
				xReturn = xSemaphoreTake(BinarySem3_Handle,40);
//				if(x_error==0)
//				{
//				ri++;
//				if(ri>20)
//				{
//				lyft_basic(0,0,0);
//				break;
//				}
//				}
//				else
//				ri=0;
				lyft_basic(-0.6,-PidLocCtrl(&X,x_error,0),0);
				xReturn = xSemaphoreTake(BinarySem5_Handle,0);
				if(NULL!=xReturn)
					Pestate=PUSH_DETECT;				
				break;
			case PUSH_DETECT:	
				xReturn = xSemaphoreTake(BinarySem5_Handle,0);
				if(NULL!=xReturn){
					saw=aw;
					Pestate=PUSH_ROLL;}				
				break;
			case PUSH_ROLL:
			xReturn = xSemaphoreTake(BinarySem3_Handle,40);
			if(saw){
			lyft_basic(0,-PidLocCtrl(&X,x_error,0),1);
			x_error=0;
			if(anglez>cps+PI/2)
				Pestate=PUSH_PUSH;}
			else{
				lyft_basic(0,-PidLocCtrl(&X,x_error,0),-1);
				x_error=0;
				if(anglez<cps-PI/2)
				Pestate=PUSH_PUSH;}
			break;
			case PUSH_PUSH:
			lyft_basic(-0.6,0,0);
			push_encoder+=M2;
			if(push_encoder>=15000)
				Pestate=PUSH_ROLLBACK;	

			break;
			case PUSH_ROLLBACK:
			if(saw){
			lyft_basic(0,0,-1);
			if(anglez<=cps)
				Pestate=PUSH_TRANSLATE;
			}
			else{
			lyft_basic(0,0,1);
			if(anglez>=cps)
				Pestate=PUSH_TRANSLATE;
			}
			break;
			case PUSH_TRANSLATE:
			if(saw)
			lyft_basic(0,0.6,0);
			else
			lyft_basic(0,-0.6,0);
			if(ipts0_num>=5||ipts1_num>=5)
				Pestate=PUSH_COMPLETE;
			break;
			case PUSH_COMPLETE:
			push_encoder=0;
			isuspend=false;
			vTaskResume(System_identify_Task_Handle);
			vTaskResume(Brake_Task_Handle);
			vTaskResume(Element_Task_Handle);
			xReturn = xSemaphoreTake(BinarySem3_Handle,0);
			Pestate=PUSH_IDLE;
			break;				
		}
		vTaskDelay(5);
	}
}
void Test_Task(void* parameter){
	BaseType_t xReturn = pdPASS;
	uint8 f =0;
	bool i=false;
	float lst=cps;
	float llst=lst;
	while(1){	
		
//		key_scanner();
//		
//		if(key_get_state(KEY_3)==KEY_SHORT_PRESS){
//			f=1;
//			anglez=0;}
//		
//		if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
//		{
//		f=0;
//		i=!i;
//		}	
//			
//		if(f==0){
		box_check();
		cps=angle_compaste();//}
//		cps=LowPass_gyro(cps,lst,llst);
//		lst=cps;
//		llst=lst;}		
//		else{
//		xReturn = xSemaphoreTake(BinarySem4_Handle,portMAX_DELAY);
//		if(i)
//		{
//		if(anglez>cps+PI/2){
//		lyft_basic(0,0,0);}
//		else{
//		xReturn = xSemaphoreTake(BinarySem3_Handle,portMAX_DELAY);
//		lyft_basic(0,-PidLocCtrl(&X,x_error,0),1);
//		}
//		}
//		else
//		{
//		if(anglez<cps-PI/2){
//		lyft_basic(0,0,0);}
//		else{
//		xReturn = xSemaphoreTake(BinarySem3_Handle,portMAX_DELAY);
//		lyft_basic(0,-PidLocCtrl(&X,x_error,0),-1);
//		}
//		}
//		x_error=0;
//		}
		vTaskDelay(5);
	}
}
void PIT_IRQHandler(void){
		BaseType_t xReturn = pdPASS;
    if(pit_flag_get(PIT_CH3))
    {
		pit_flag_clear(PIT_CH3);
		get_encoder();
		L2_gyro_z = L_gyro_z;
		L_gyro_z = new_gyro_z;
		imu963ra_get_gyro();
		new_gyro_z = imu963ra_gyro_transition(imu963ra_gyro_z);             
		ta = LowPass_gyro(new_gyro_z, L_gyro_z, L2_gyro_z)*0.005;
		if(fabs(ta)>0.03)
		anglez+=ta*PI/180;
		dangle= anglez-langle;
		langle= anglez;
		xReturn = xSemaphoreGive( BinarySem2_Handle );
		xReturn = xSemaphoreGive( BinarySem4_Handle );
    }
}
void LPUART1_IRQHandler(void){
		BaseType_t xReturn = pdPASS;
		uint8 temp=0;
		while(pi==7)
		{memset(judge_uart_buffer,0,7);
		pi=0;
		}
    if(kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(LPUART1))
	{	
		uart_query_byte(UART_1,&temp);
		if(temp==255)
		art_flag=temp;
		else{	
			judge_uart_buffer[pi++]=temp;
			if(judge_uart_buffer[0]==200)
			{
				if(judge_uart_buffer[1]==0)
					x_error=(int)judge_uart_buffer[2];
				else
					x_error=-(int)judge_uart_buffer[2];
				if(judge_uart_buffer[3]==205)
				{		
					if(judge_uart_buffer[4]==0)
						y_error=(int)judge_uart_buffer[5];
					else
						y_error=-(int)judge_uart_buffer[5];
				}
				if(judge_uart_buffer[6]==210)
				{
					xReturn = xSemaphoreGive(BinarySem3_Handle);
				}		
			}
			else;
			
			}
	}    
    LPUART_ClearStatusFlags(LPUART1, kLPUART_RxOverrunFlag);    // 不允许删除
}
void LPUART4_IRQHandler(void){		
		BaseType_t xReturn = pdPASS;
		uint8 temp=0;
		while(pi1==5)
		{memset(class_uart_buffer,0,5);
		pi1=0;
		}
    if(kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(LPUART4))
    {
       uart_query_byte(UART_4,&temp);
		if(temp==255)
		art1_flag=temp;
		else{	
			class_uart_buffer[pi1++]=temp;
			if(class_uart_buffer[0]==0x14){
					aw=class_uart_buffer[1];
				if(class_uart_buffer[2]==0x15)				
						aindex=class_uart_buffer[3];
				if(class_uart_buffer[4]==0x16)
					xReturn = xSemaphoreGive(BinarySem5_Handle);	
				;
			}
			else;
    }
	}  
    LPUART_ClearStatusFlags(LPUART4, kLPUART_RxOverrunFlag);    // 不允许删除
}
void PrintTaskList(void) {
    char buffer[256];
    vTaskList(buffer); 
	printf("=================================================\r\n");
	printf("任务名        任务状态 优先级  剩余栈  任务号 \r\n");
    printf("Task List:\r\n%s\n", buffer);
	printf("  B：阻塞  R：就绪  D：删除  S：暂停  X：运行 \r\n");
}
void PrintRunTimeStats(void) {
    char buffer[256];
    vTaskGetRunTimeStats(buffer); 
	printf("=================================================\r\n");
	printf("任务名         运行计数        CPU使用率 \r\n");
    printf("Run Time Stats:\r\n%s\n", buffer);
}
void configureTimerForRunTimeStats() {
	
    timer_init(GPT_TIM_1,TIMER_MS);
	timer_start(GPT_TIM_1);
}
uint32_t getRunTimeCounterValue() {
    
    return timer_get(GPT_TIM_1);
}
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ){
	Motor_Init();
	ips200_show_string(0,160,"stackoverflow");
}