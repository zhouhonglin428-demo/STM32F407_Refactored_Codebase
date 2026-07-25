#ifndef _BSP_DMA_H
#define _BSP_DMA_H

#include "stm32f4xx.h"


//DMA
#define DEBUG_USART_DR_BASE               (USART1_BASE+0x04)		
#define SENDBUFF_SIZE                     5000				//发送的数据量
#define DEBUG_USART_DMA_CLK               RCC_AHB1Periph_DMA2	
#define DEBUG_USART_DMA_CHANNEL           DMA_Channel_4
#define DEBUG_USART_DMA_STREAM            DMA2_Stream7

extern uint8_t SendBuff[SENDBUFF_SIZE];


void DMA_Config(void);
void Delay(__IO uint32_t nCount);

#endif /* _BSP_DMA_H */

