#include "stm32f10x.h"                  // Device header
uint16_t RX_data;
void USART2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//串口外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO外设时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;//初始化USART2
	USART_InitStructure.USART_BaudRate=115200;//波特率.
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长，这里选8位。
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位，这里选择一个
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位，这里选择无校验位。
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//串口模式，发送接收。
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//指定了硬件流控制模式使能还是失能,这里选择失能。
	USART_Init(USART2,&USART_InitStructure);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能USART2,选择中断标志位，选择使能。
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组NVIC
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//选择USART2中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//该参数设置了成员 NVIC_IRQChannel 中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//该参数设置了成员 NVIC_IRQChannel 中的从优先级
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2,ENABLE);
	
}

void USART2_SEND_data(uint8_t data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待数据发送完成。该标志位置1之后，不需要手动清零。

}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		RX_data=USART_ReceiveData(USART2);//接收
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除标志位
		
	}
}
