/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library ����RT1064DVL6A ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RT1064DVL6A ��Դ���һ����
* 
* RT1064DVL6A ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 8.32.4 or MDK 5.33
* ����ƽ̨          RT1064DVL6A
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/
#include "para_space.h"
// �������ǿ�Դ����ֲ�ÿչ���
/*      ÿ��һ�� �ù�ʡ��             ����ΰ� �ȹ�����        */
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
// // //      ���汣��      ����BUG      �����޸�          \\ \\ \\
// // // // // // || || || || || || || || || || \\ \\ \\ \\ \\ */
float ta,L2_gyro_z,L_gyro_z,new_gyro_z;

float dangle,langle,w;
//
extern int16 circle_encoder;


TaskHandle_t AppTaskCreate_Handle = NULL;

TaskHandle_t IPS_Task_Handle = NULL;
TaskHandle_t Process_image_Task_Handle= NULL;
TaskHandle_t Find_corners_Task_Handle= NULL;
TaskHandle_t System_identify_Task_Handle= NULL;
TaskHandle_t Brake_Task_Handle= NULL;
TaskHandle_t Key_Task_Handle= NULL;
TaskHandle_t Element_Task_Handle= NULL;
TaskHandle_t Alloc_Task_Handle= NULL;

void ipsTask(void* parameter);
void process_image_Task(void * parameter);
void find_corners_Task(void* parameter);
void System_Identify_Task(void* parameter);
void Brake_Task(void* parameter);
void Key_Task(void* parameter);
void Element_Task(void* parameter);
void Alloc_Task(void* parameter);

void AppTaskCreate(void);
void PrintTaskList(void);
void PrintRunTimeStats(void);

SemaphoreHandle_t BinarySem0_Handle =NULL;
SemaphoreHandle_t BinarySem1_Handle =NULL;

int main(void){
    clock_init(SYSTEM_CLOCK_600M);  // ����ɾ��
    debug_init();                  // ���Զ˿ڳ�ʼ��
    // �˴���д�û����� ���������ʼ�������
		wireless_uart_init();
		system_delay_ms(300);
		//my_spi_wifi_init();
		timer_init(GPT_TIM_1, TIMER_MS);
		interrupt_set_priority(PIT_IRQn, 0);
		interrupt_global_enable(0);
	
		gpio_init(B11,GPO,0,GPO_PUSH_PULL);
	
		imu963ra_init ();
		encoder_init();
		pit_ms_init(PIT_CH1,2);
		pit_ms_init(PIT_CH3,5);
			
		ips200_set_dir(IPS200_CROSSWISE_180);
		ips200_init(IPS200_TYPE_SPI);
		system_delay_ms(300);
		key_init(10);    
		Motor_Init();		
		while(mt9v03x_init())
		{
			printf("camera Init error");
		}

		
//		my_spi_wifi_image_init();

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

//  xReturn = xTaskCreate((TaskFunction_t )ipsTask,  
//                        (const char*    )"IPS_Task",
//                        (uint16_t       )256,  
//                        (void*          )NULL,
//                        (UBaseType_t    )3, 
//                        (TaskHandle_t*  )&IPS_Task_Handle);
//  if(pdPASS == xReturn)
//    printf("����IPS_Task����ɹ�!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )System_Identify_Task,  
                        (const char*    )"System_identify_Task",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&System_identify_Task_Handle);
  if(pdPASS == xReturn)
    printf("����SI����ɹ�!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )process_image_Task,  
                        (const char*    )"process_image_Task",
                        (uint16_t       )192,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )&Process_image_Task_Handle);
  if(pdPASS == xReturn)
    printf("����process_image����ɹ�!\r\n");
	xReturn = xTaskCreate((TaskFunction_t )Brake_Task,  
                        (const char*    )"Brake_Task",
                        (uint16_t       )256,  
                        (void*          )NULL,
                        (UBaseType_t    )1, 
                        (TaskHandle_t*  )&Brake_Task_Handle);
  if(pdPASS == xReturn)
    printf("����Brake����ɹ�!\r\n");
	xReturn = xTaskCreate((TaskFunction_t )Key_Task,  
                        (const char*    )"Key_Task",
                        (uint16_t       )128,  
                        (void*          )NULL,
                        (UBaseType_t    )3, 
                        (TaskHandle_t*  )&Key_Task_Handle);
  if(pdPASS == xReturn)
    printf("����Key����ɹ�!\r\n");

	xReturn = xTaskCreate((TaskFunction_t )find_corners_Task,  
                        (const char*    )"Find_corners_Task",
                        (uint16_t       )192,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )Find_corners_Task_Handle);
  if(pdPASS == xReturn)
    printf("����Find_corners����ɹ�!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )Element_Task,  
                        (const char*    )"Element_Task",
                        (uint16_t       )384,  
                        (void*          )NULL,
                        (UBaseType_t    )2, 
                        (TaskHandle_t*  )Element_Task_Handle);
  if(pdPASS == xReturn)
    printf("����Element����ɹ�!\r\n");
	
//	xReturn = xTaskCreate((TaskFunction_t )Alloc_Task,  
//                        (const char*    )"Alloc_Task",
//                        (uint16_t       )256,  
//                        (void*          )NULL,
//                        (UBaseType_t    )3, 
//                        (TaskHandle_t*  )& Alloc_Task_Handle);
//  if(pdPASS == xReturn)
//    printf("����Alloc����ɹ�!\r\n");
	
  vTaskDelete(AppTaskCreate_Handle); 
  
  taskEXIT_CRITICAL();            
}

void ipsTask(void* parameter){
	while(1)
	{		
		ips200_show_gray_image(0,0,show0[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
//		draw();
		vTaskDelay(20);
	}
}

void System_Identify_Task(void* parameter){
	int i=0;
	uint8 s=0,ls=0;
	vTaskSuspend(System_identify_Task_Handle);
	while(1)
	{
					float a0=calculator(aim0),a1=calculator(aim1),a=a0+a1;
		
					if(a>=0)					
						s=1;
					else
						s=0;
					if(s-ls!=0)
						Gangle.out_i=0;						
						w=dangle*500.f;
						if(track_type == TRACK_MIX)
						lyft_basic( -0.4,0,1*func_limit_ab(PidLocCtrl(&Gangle,dangle,a),-8.f,8.f));
						
						else if(track_type == TRACK_LEFT)
						{
							lyft_basic(-0.4,0,0.5*func_limit_ab(PidLocCtrl(&Langle,dangle,a0),-8.f,8.f));
						}else if(track_type == TRACK_RIGHT)
						{
							lyft_basic(-0.4,0,2.6*func_limit_ab(PidLocCtrl(&Rangle,dangle,a1),-8.f,8.f));
						}
						ls=s;
						//m1speed(21.57);
						JF_Data.data[2]=Rangle.out;
						JF_Data.data[1]=Langle.out ;
						JF_Data.data[0]=Gangle.out;
 						JustFloat_Send();	
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
				if(cnt>10||status)
				{
					if(status)
					ips200_show_string(0,163,"Stopline");
					else
					ips200_show_string(0,163,"Stopnone");	
					lyft_basic(0,0,0);
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
	while(1)
	{
		key_scanner();
		if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
		{
			PID_Init(&m1);
			PID_Init(&m2);
			PID_Init(&m3);
			PID_Init(&Gangle);
			vTaskResume(System_identify_Task_Handle);
		}
		vTaskDelay(10);
	}
}
	
void process_image_Task(void * parameter) {
		ImagePerspective_Init();
		BaseType_t xReturn = pdPASS;
		while(1)
		{
			if(mt9v03x_finish_flag)
        {
          mt9v03x_finish_flag=0;
					IPMcopy();
					blur(&img_raw,&img_AP,5);
					clone_image(&img_AP,&img_raw);
			
					int x1 = img_raw.width / 2 - begin_x, y1 = begin_y;
					ipts0_num = sizeof(ipts0) / sizeof(ipts0[0]);
					for (; x1 > 0; x1--) if (AT_IMAGE(&img_raw, x1 - 1, y1) < thres) break;
					if (AT_IMAGE(&img_raw, x1, y1) >= thres)
					findline_lefthand_adaptive(&img_raw, block_size, clip_value, x1, y1-1, ipts0, &ipts0_num);
					else ipts0_num=0;
		
					int x2 = img_raw.width / 2 + begin_x, y2 = begin_y;
					ipts1_num = sizeof(ipts1) / sizeof(ipts1[0]);
					for (; x2 < img_raw.width - 1; x2++) if (AT_IMAGE(&img_raw, x2 + 1, y2) < thres) break;
					if (AT_IMAGE(&img_raw, x2, y2) >= thres)
							findline_righthand_adaptive(&img_raw, block_size, clip_value, x2, y2, ipts1, &ipts1_num);
					else ipts1_num=0;
		
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
        if (Lpt0_found == false && 70. / 180. * PI < conf && conf < 140. / 180. * PI && i < 0.8 / sample_dist) {
            Lpt0_rpts0s_id = i;
            Lpt0_found = true;
        }
        //straight road 
        if (conf > 5. / 180. * PI && i < 1.4 / sample_dist) is_straight0 = false;
        if (Lpt0_found == true && is_straight0 == false) break;
    }
    for (int i = 0; i < rpts1s_num; i++) {
        if (rpts1an[i] == 0) continue;
        int im1 = clip(i - (int) round(angle_dist / sample_dist), 0, rpts1s_num - 1);
        int ip1 = clip(i + (int) round(angle_dist / sample_dist), 0, rpts1s_num - 1);
        float conf = fabs(rpts1a[i]) - (fabs(rpts1a[im1]) + fabs(rpts1a[ip1])) / 2;
       
        if (Lpt1_found == false && 70. / 180. * PI < conf && conf < 140. / 180. * PI && i < 0.8 / sample_dist) {
            Lpt1_rpts1s_id = i;
            Lpt1_found = true;
        }

        if (conf > 5. / 180. * PI && i < 1.4 / sample_dist) is_straight1 = false;

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
//		if(Lpt0_found && Lpt1_found)
//		{
//			gpio_set_level(B11,1);
//			vTaskDelay(500);
//			gpio_set_level(B11,0);
//		}

		vTaskDelay(5);
	}
}


void Element_Task(void* parameter){
	BaseType_t xReturn = pdPASS;
	while(1)
	{
		xReturn = xSemaphoreTake(BinarySem1_Handle,
                              portMAX_DELAY);	
		check_circle();
		run_circle();
		ips200_show_uint(0,195,circle_type,3);
		vTaskDelay(5);					
	}	
}


void Alloc_Task(void* parameter){
	while(1)
	{
		PrintRunTimeStats();				
		PrintTaskList();
		vTaskDelay(2000);
	}
}


void PIT_IRQHandler(void){
    if(pit_flag_get(PIT_CH3))
    {
      pit_flag_clear(PIT_CH3);
			get_encoder();
    }
		if(pit_flag_get(PIT_CH1))
    {
      pit_flag_clear(PIT_CH1);
			#if 1
			L2_gyro_z = L_gyro_z;
			L_gyro_z = new_gyro_z;
			imu963ra_get_gyro();
			new_gyro_z = imu963ra_gyro_transition(imu963ra_gyro_z);             
			ta = LowPass_gyro(new_gyro_z, L_gyro_z, L2_gyro_z)*0.002;
			if(fabs(ta)>0.02)
			anglez+=ta*PI/180;
			dangle= anglez-langle;
			langle= anglez;
			#endif
    }
}
void PrintTaskList(void) {
    char buffer[256];
    vTaskList(buffer); 
		printf("=================================================\r\n");
		printf("������        ����״̬ ���ȼ�  ʣ��ջ  ����� \r\n");
    printf("Task List:\r\n%s\n", buffer);
		printf("  B������  R������  D��ɾ��  S����ͣ  X������ \r\n");
}
void PrintRunTimeStats(void) {
    char buffer[256];
    vTaskGetRunTimeStats(buffer); 
		printf("=================================================\r\n");
		printf("������         ���м���        CPUʹ���� \r\n");
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