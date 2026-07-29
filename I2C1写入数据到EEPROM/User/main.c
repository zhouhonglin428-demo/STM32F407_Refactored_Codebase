#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_iic_ee.h"

int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  Debug_USART_Config();

	/* 发送一个字符串 */
	printf("这是一个EEPROM读写实验\n");

	EEPROM_IIC_Config();
	printf("初始化完成\n");
	
	//写入测试
	EEPROM_Byte_Write(0x00,0x12);
	
	
	printf("写入测试结束\n");
	while(1)
		{	
		
		}	
}



/*********************************************END OF FILE**********************/

