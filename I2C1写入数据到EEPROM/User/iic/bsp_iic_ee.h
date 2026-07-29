#ifndef _BSP_IIC_EE_H
#define _BSP_IIC_EE_H


#include "stm32f4xx.h"
#include <stdio.h>
void EEPROM_IIC_Config(void);
void EEPROM_Byte_Write(uint8_t addr,uint8_t data);

//Òý½Å¶¨Òå
#define EEPROM_I2C1_ADDR						0xA0


#define EEPROM_I2C1_ClockSpeed					400000









#endif /* _BSP_IIC_EE_H */
