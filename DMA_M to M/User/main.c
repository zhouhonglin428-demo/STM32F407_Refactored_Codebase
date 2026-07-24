#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_dma.h"

uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength);
void DMA_Config(void);
							

int main(void)
{
	//led初始化
	LED_GPIO_Config();
	
	/* 设置RGB彩色灯为紫色 */
  LED_PURPLE;  
		
	
  /* 简单延时函数 */
  Delay(0xFFFFFF);  
  
  /* DMA传输配置 */
  DMA_Config(); 
	
	// 一行代码完成传输并获取比较结果
    if (DMA_M2M_TransferAndCheck()) {
        LED_BLUE;   // 成功点蓝灯
    } else {
        LED_RED;    // 失败点红灯
    }

    while (1);
	 
 
  
  while (1)
	{		
	}
	
}







