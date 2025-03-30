#include "zf_common_headfile.h"
//ѡ��Э��,��ѡһ������Ȼû�취ͨ��
//#define UTP
#define TCP
#define WIFI_SSID_TEST  "111"
#define WIFI_PASSWORD_TEST   "12345678"
uint8 wifi_spi_test_buffer[] = "��ʼ���ɹ�\n";
uint8 wifi_spi_get_data_buffer[256];
uint32 data_length = 0;

extern uint8_t show0[100][114] ;


/*  @wifi��ʼ������
 *  @˵����������ɿ��޸Ķ���
 *  @���ߣ�����
 *  @ʱ�䣺2024��3��7��
 * */
void my_spi_wifi_init(void)
{
    while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
		printf("\r\n connect wifi failed. \r\n");
        system_delay_ms(100);                                                   // ��ʼ��ʧ�� �ȴ� 100ms
    }
    
    printf("\r\n module version:%s", wifi_spi_version);      					// ģ��̼��汾
    printf("\r\n module mac    :%s", wifi_spi_mac_addr);     					// ģ�� MAC ��Ϣ
    printf("\r\n module ip     :%s", wifi_spi_ip_addr_port); 					// ģ�� IP ��ַ

    // zf_device_wifi_spi.h �ļ��ڵĺ궨����Ը���ģ������(����) WIFI ֮���Ƿ��Զ����� TCP ������������ UDP ����
    if(1 != WIFI_SPI_AUTO_CONNECT)                                              // ���û�п����Զ����� ����Ҫ�ֶ�����Ŀ�� IP
    {
        while(wifi_spi_socket_connect(                                          // ��ָ��Ŀ�� IP �Ķ˿ڽ��� TCP ����
            "TCP",                                                              // ָ��ʹ��TCP��ʽͨѶ
            WIFI_SPI_TARGET_IP,                                                 // ָ��Զ�˵�IP��ַ����д��λ����IP��ַ
            WIFI_SPI_TARGET_PORT,                                               // ָ��Զ�˵Ķ˿ںţ���д��λ���Ķ˿ںţ�ͨ����λ��Ĭ����8080
            WIFI_SPI_LOCAL_PORT))                                               // ָ�������Ķ˿ں�
        {
            // ���һֱ����ʧ�� ����һ���ǲ���û�н�Ӳ����λ
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // ��������ʧ�� �ȴ� 100ms
        }
    }
 
 
}

/*  @ͼ���ͺ�����ʼ��
 *  @˵����������ɿ��޸Ķ���
 *  @���ߣ�����
 *  @ʱ�䣺2024��3��7��
 * */
void my_spi_wifi_image_init(void)
{
    //��ʼ�������λ�����֣��Ƽ�������ͷ��ʼ����
    // ������ֳ�ʼ�� ���ݴ���ʹ�ø���WIFI SPI
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
    //����ͼ������
    // ���������ͼ����Ϣ(������ԭʼͼ����Ϣ)
   seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
 
}

void my_spi_wifi_image_send(void)
{
    seekfree_assistant_camera_send();

}
