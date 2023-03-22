#ifndef _USTAR111_H
#define _USTAR111_H
#include "stm32f10x.h"// Device header

extern uint16_t RX_data;//数据接收变量
void USART2_Init(void);
void USART2_SEND_data(uint8_t data);
void SEND_ARR(uint8_t* array,uint16_t Length);//发送数组,数据、元素个数.


#endif
