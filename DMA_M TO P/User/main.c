#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_dma.h"



static void Delay(__IO uint32_t nCount);

int main(void)
{
	/* 编写开启串口发送DMA请求 */
	/* 初始化USART */
	Debug_USART_Config(); 

	/* 配置使用DMA模式 */
	DMA_Config();

	/* 配置RGB彩色灯 */
	LED_GPIO_Config();
	
	printf("\r\n USART1 DMA TX 测试 \r\n");
	
	uint16_t i;
	/*1.填充数据 */
	for(i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A';
  
	}
	
	
	/* 3. 开启串口 DMA 请求 */
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);
	
	/* 2. 数据填充完成，正式使能DMA ) */
	DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);
	

	/* 此时CPU是空闲的，可以干其他的事情 */  
	//例如同时控制LED	
	
	while (1)
	{
	LED1_TOGGLE
    Delay(0xFFFFF);
	}
}


static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



/*********************************************END OF FILE**********************/

