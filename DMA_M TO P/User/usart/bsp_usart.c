#include "bsp_usart.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* 配置USART为中断源 */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//  /* 抢断优先级为1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* 子优先级为1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* 使能中断 */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* 初始化配置NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}




void Debug_USART_Config(void)
{
	/* 第一步：初始化串口所需要用到的GPIO，GPIO_InitTypeDef,GPIO_PinAFConfig() */
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
  
	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚为复用功能，因为RX是接收所以不需要配置输出类型及输出速度	 */
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 建立引脚与外设的具体映射 */
	/* 把物理接收引脚PA10，复用连接到RX */
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

	/* 把物理发送引脚PA9， 复用连接到TX*/
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);	
	
		
	/* 第二步：初始化串口USART,USART_InitTypeDef */ 
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	
	/* 配置串DEBUG_USART 模式 */
	/* 波特率设置：DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	/* 字长(数据位+校验位)：8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* 停止位：1个停止位 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* 校验位选择：不使用校验 */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* 硬件流控制：不使用硬件流 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART模式控制：同时使能接收和发送 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 完成USART初始化配置 */
	USART_Init(DEBUG_USART, &USART_InitStructure); 
		
//	
//	/* 第三步：中断配置 */
//	
//	/* 嵌套向量中断控制器组选择 */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	/* 嵌套向量中断控制器NVIC配置 */
//	NVIC_Configuration();
//	/* 使能串口接收中断 */
//	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
	
	/* 第四步：使能串口 */
	
	/* 使能串口 */
	USART_Cmd(DEBUG_USART, ENABLE);	
}


	/* 第五步：编写发送和接收函数 */


/*
如果硬件还在忙着搬运上一个数据，这个标志位就是 0（RESET），意思是“还没空出来，别塞新数据”。
如果硬件搬运完了，寄存器空了，它会自动变成 1（SET）。
*/



/*
再多写Usart_SendByte函数的目的就是
因为单片机的速度很快，硬件跟不上
为了发送数据之后能够等待数据
这样子一下子传10个数据的时候就会一个一个安全的发过去
这样子数据就不会被覆盖了
*/
/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void Usart_SendString(USART_TypeDef* pUSARTx, char *str)
{
	unsigned int k=0;
	
	do 
	{
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
	} while(*(str + k)!='\0');
	
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);	
}



///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}



///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		//等待串口输入数据 
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}

void USART_DMA_Config(void)
{
	void DMA_Config(void);
}







