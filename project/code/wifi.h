#ifndef __MYSPIWIFI_H_
#define __MYSPIWIFI_H_
#include "zf_common_headfile.h"

void my_spi_wifi_init(void);
uint32 my_spi_wifi_rece(void);
void my_spi_wifi_image_init(void);
void  my_spi_wifi_image_send (void);
#endif