#include "zf_common_headfile.h"
#include "vofa.h"


//-----------------------------------------------------------------------------------------------
// 函数简介  FireWater数据引擎
// 参数说明  
// 返回参数  void
// 使用示例  ->
// int8    data_1 = -13;
// uint16  data_2 = 234;
// float   data_3 = 3.4;
// FW_Data[0].type = 'd';
// FW_Data[1].type = 'd';
// FW_Data[2].type = 'f';
// FW_Data[0].int_data   = data_1;
// FW_Data[1].int_data   = data_2;
// FW_Data[2].float_data = data_3;
// FireWater_Send();
// 备注信息  1.其中FW_Data[x].type可以不放在主循环或中断中
// 备注信息  2.
// 不要忘了给数据类型(.type)赋值 ,  .int_data 或 .float_data要与.type匹配!
// 不要忘了给数据类型(.type)赋值 ,  .int_data 或 .float_data要与.type匹配!
// 不要忘了给数据类型(.type)赋值 ,  .int_data 或 .float_data要与.type匹配!
//-----------------------------------------------------------------------------------------------
static char data_str[12];
FireWater_Struct FW_Data[CH_COUNT] = {[0 ... CH_COUNT-1] = {0, 0, 'd'}};//特殊初始化语法，将索引0到CH_COUNT-1的元素初始化为0, 0.0, 'd'}

void FireWater_Send()
{
    for(uint8 i = 0; i < CH_COUNT; i++)
    {
        switch (FW_Data[i].type) 
        {
            case 'd':
                snprintf(data_str, sizeof(data_str), "%d", FW_Data[i].int_data);
                break;
            case 'f':
                snprintf(data_str, sizeof(data_str), "%f", FW_Data[i].float_data);
                break;			
            default:
                wireless_uart_send_string("error: Unknown type!\n");
                continue;
        }
        if(i != CH_COUNT-1)
        {
            wireless_uart_send_string(data_str);
            wireless_uart_send_string(",");
        }
        else
        {
            wireless_uart_send_string(data_str);
            wireless_uart_send_string("\n");
        }
    }
}


//-----------------------------------------------------------------------------------------------
// 函数简介  JustFloat数据引擎
// 参数说明  
// 返回参数  void
// 使用示例  :
//JF_Data.data[0] = Kp_T;
//JF_Data.data[1] = Kd_T;
//JF_Data.data[2] = 0.45;
//JF_Data.data[3] = -3.36;
//JustFloat_Send();
// 备注信息  
//-----------------------------------------------------------------------------------------------
JustFloat_Struct JF_Data;

void JustFloat_Send()
{
    JF_Data.tail[0] = 0x00;
	JF_Data.tail[1] = 0x00;
	JF_Data.tail[2] = 0x80;
	JF_Data.tail[3] = 0x7f;
  wireless_uart_send_buffer((const uint8*)JF_Data.data, sizeof(float) * CH_COUNT);
 	wireless_uart_send_buffer((const uint8*)JF_Data.tail, 4);
}
