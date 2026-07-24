#ifndef _BSP_DMA_H
#define _BSP_DMA_H

#include "stm32f4xx.h"

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              10000 /* Maximum timeout value */
/* 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定 */
/* 要想使用存储器到存储器，必须选择DMA2 */
#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_Channel_0
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_FLAG_TCIF            DMA_FLAG_TCIF0




// 声明外部变量，让 main.c 可以访问
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

void Delay(__IO uint32_t nCount);
void DMA_Config(void);
uint8_t DMA_M2M_TransferAndCheck(void);

#endif /* _BSP_DMA_H */

