#include "bsp_dma.h"
#include "bsp_led.h"


//uint8_t SendBuff[SENDBUFF_SIZE] = // 直接赋予初始内容（比如你想发一堆 'A'）
uint8_t SendBuff[SENDBUFF_SIZE];
																	
/* DMA传输配置 */
void DMA_Config(void)
{
	 DMA_InitTypeDef  DMA_InitStructure;
	
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK,ENABLE);
	
	/* 确保清空历史残余配置完成，防止影响寄存器的当前状态，进而导致配置失或数据传输异常 */
	/* 复位初始化DMA数据流 */
	DMA_DeInit(DEBUG_USART_DMA_STREAM);

	/* 确保DMA数据流复位完成 */
	while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != DISABLE)
	{
	}
	/*usart1 tx对应dma2，通道4，数据流7*/	
	/* DMA通道数据流选择 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	/* 1. 设置DMA传输方向：从内存到外设（Memory -> Peripheral）*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;//Direction（传输方向）
	/* 2. 设置数据源：内存缓冲区地址 */
	DMA_InitStructure.DMA_Memory0BaseAddr = DEBUG_USART_DR_BASE; 	
	/* 3. 设置数据目标：外设串口数据寄存器（DR）地址 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SendBuff;
	/* 数据数目 */
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	/*这个Peripheral指的是外设 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//Inc (Increment)：地址递增 / 自增
	/* 外设地址不增 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;
	/* 源数据是字大小(32位) */
	/* 每次去搬运数据时，一口气吃掉/写入多少个字节(数据宽度)*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* 目标数据) */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* DMA模式：一次循环 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/* DMA数据流优先级为高 */
	DMA_InitStructure.DMA_Priority	= DMA_Priority_High;
	/* 禁用FIFO模式,因为此时是USART传输 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	/* 单次模式 */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/* 传输完成 */
	DMA_Init(DEBUG_USART_DMA_STREAM,&DMA_InitStructure);
  
	/* 清除DMA数据流传输完成标志位 */
	DMA_ClearFlag(DEBUG_USART_DMA_STREAM,DMA_FLAG_TCIF7);
//  
//	/* 使能DMA数据流，开始DMA数据传输 */
//	DMA_Cmd(DEBUG_USART_DMA_STREAM,ENABLE);
//  
//	/* 等待DMA数据流有效 */
//	while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE )
//	{
//	}

}



