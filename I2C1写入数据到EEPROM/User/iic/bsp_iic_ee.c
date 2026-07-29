/* brief:EEPROM驱动程序 */


#include "bsp_iic_ee.h"

/*
实现往EEPROM写入数据。
1.初始化硬件I2C
2.编写往EEPROM传输数据的函数（包括读和写）
3.利用读写函数往EEPROM写入数据，并读取数据进行校验
*/

/* EEPROM_IIC_Config初始化GPIO和I2C模式 */
void EEPROM_IIC_Config(void)
{
	/* I2C必须使用开漏模式 */
	/* I2C外设SCL连接到PB8，SDA连接到PB9 */
	/* 第一步：开GPIOB时钟和I2C1时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	/* 第二步：引脚复用映射 */
	/* 连接GPIO到I2C */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	
	/* 第三步：配置GPIO并Init(每个引脚都需要配置结构体参数) */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	/* 第四步：配置I2C并Init */
	I2C_InitTypeDef I2C_InitStructure;
	
	//使能或关闭响应
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	//指定地址的长度
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//设置SCL时钟频率
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C1_ClockSpeed;
	//指定时钟占空比
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//指定I2C模式
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ;
	//指定自身的I2C设备地址
	I2C_InitStructure.I2C_OwnAddress1 = 0x78;//随便一个设备地址，反正是主机的地址，不要跟其他设备地址相同就ok
	
	I2C_Init(I2C1,&I2C_InitStructure);
	
	
	/* 第五步：使能I2C */
	I2C_Cmd(I2C1,ENABLE);
}



/* 编写往EEPROM传输数据的函数（包括读和写） */
/* addr:要写入存储单元的地址 */
/* data:要写入的数据 */

void EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
	}
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
	}

	/* 发送要写入存储单元的地址 */
	I2C_SendData(I2C1,addr);
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
	}
	
	/* 发送要写入的数据 */
	I2C_SendData(I2C1,data);
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
	}
	
	/* 产生结束信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);

	
	/* 因为stm32的操作速度非常快 ，但是I2C的速度相比于I2C会慢 */
	/* 例如stm32发送完起始信号之后，立马就去发送EEPROM地址了。所以要等待I2C进行响应 */

}
