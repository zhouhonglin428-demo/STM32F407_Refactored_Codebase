#include "bsp_dma.h"
#include "bsp_led.h"

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};


/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];
																	
/* DMA传输配置 */
void DMA_Config(void)
{
	 DMA_InitTypeDef  DMA_InitStructure;
	 __IO uint32_t    Timeout = TIMEOUT_MAX;
	
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	/* 确保清空历史残余配置完成，防止影响寄存器的当前状态，进而导致配置失或数据传输异常 */
	/* 复位初始化DMA数据流 */
  DMA_DeInit(DMA_STREAM);

  /* 确保DMA数据流复位完成 */
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE)
  {
  }
	/* DMA通道数据流选择 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	/* 这个传的是地址*/
	/* 源数据地址 */
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aDST_Buffer;
	/* 目标地址 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	/* 存储器到存储器模式 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;//Direction（传输方向）
	/* 数据数目 */
	DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
	/*这个Peripheral虽然指的是外设，但是在存储器到存储器模式下，
	DMA 把外设接口当成了第二个内存来用*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//Inc (Increment)：地址递增 / 自增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable ;
	/* 源数据是字大小(32位) */
	/* 每次去搬运数据时，一口气吃掉/写入多少个字节(数据宽度)*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	/* 目标数据也是字大小(32位) */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
	/* 一次传输模式，存储器到存储器模式不能使用循环传输 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/* DMA数据流优先级为高 */
	DMA_InitStructure.DMA_Priority	= DMA_Priority_High;
	/* 使用FIFO模式，当使用M to M模式时，FIFO硬件自动开启，软件控制不了 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	/* 单次模式 */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/* 传输完成 */
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
  
	/* 清除DMA数据流传输完成标志位 */
	DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);
  
	/* 使能DMA数据流，开始DMA数据传输 */
	DMA_Cmd(DMA2_Stream0,ENABLE);
  
	/* 检测DMA数据流是否有效并带有超时检测功能 */
	Timeout = TIMEOUT_MAX;
	while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0))
	{
	}
	
	/* 判断是否超时 */
	if (Timeout == 0)
	{
    /* 超时就让程序运行下面循环：RGB彩色灯闪烁 */
    while (1)
		{      
		LED_RED;
		Delay(0xFFFFFF);
		LED_RGBOFF;
		Delay(0xFFFFFF);
		}
	}
	
}


/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}

// 在 bsp_dma.c 里实现
uint8_t DMA_M2M_TransferAndCheck(void)
{
    // 1. 初始化并启动 DMA
    DMA_Config();
    
    // 2. 等待传输完成（将轮询逻辑从 main.c 抽走）
    while (DMA_GetFlagStatus(DMA_STREAM, DMA_FLAG_TCIF) == DISABLE);
    
    // 3. 数据校验并返回结果 (1: 成功, 0: 失败)
    return Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
}


/* 简单的延时函数 */
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}	










