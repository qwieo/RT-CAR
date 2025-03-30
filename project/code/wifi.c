#include "zf_common_headfile.h"
//选择协议,必选一个，不然没办法通信
//#define UTP
#define TCP
#define WIFI_SSID_TEST  "111"
#define WIFI_PASSWORD_TEST   "12345678"
uint8 wifi_spi_test_buffer[] = "初始化成功\n";
uint8 wifi_spi_get_data_buffer[256];
uint32 data_length = 0;

extern uint8_t show0[100][114] ;


/*  @wifi初始化函数
 *  @说明：根据逐飞库修改而来
 *  @作者：吴广金
 *  @时间：2024年3月7日
 * */
void my_spi_wifi_init(void)
{
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
		printf("\r\n connect wifi failed. \r\n");
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
    }
    
    printf("\r\n module version:%s", wifi_spi_version);      					// 模块固件版本
    printf("\r\n module mac    :%s", wifi_spi_mac_addr);     					// 模块 MAC 信息
    printf("\r\n module ip     :%s", wifi_spi_ip_addr_port); 					// 模块 IP 地址

    // zf_device_wifi_spi.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                 // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                               // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                               // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
        }
    }
 
 
}

/*  @图像发送函数初始化
 *  @说明：根据逐飞库修改而来
 *  @作者：吴广金
 *  @时间：2024年3月7日
 * */
void my_spi_wifi_image_init(void)
{
    //初始化逐飞上位机助手，推荐在摄像头初始化后
    // 逐飞助手初始化 数据传输使用高速WIFI SPI
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    //配置图传参数
    // 发送总钻风图像信息(仅包含原始图像信息)
   seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
 
}

void my_spi_wifi_image_send(void)
{
    seekfree_assistant_camera_send();

}
