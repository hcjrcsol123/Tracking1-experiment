#include "stm32f10x.h"                  // Device header
#include "Tracing1.h"
#include "GPIOandTIM.H"
#include "USART2.h"
#include "Delay.h"
//在位移函数输入点击转速、前进格子数、前进格子数。串口负责接收步骤序号，函数会判断接收到的序号是否和函数输入的序号
//一致，若一致，则执行，若不一致则跳过，执行完后，会发送完成信号，并等待下一次命令。
int main(void)
{
	
	USART2_Init();
	Tracing1_GPIO_Init();
	TIM1_PWM_Init();
	electric_machinery_PWM(75);
	RX_data = 1;
	while(1)
	{
//示例
//向前移动8格，再向左移动8格，然后向右移动5格，再向前移动8格，最后向后移动9格。

		Go_forward(Fastspeed,2,1);
		Pan_Left(Fastspeed,2,0);
		Pan_right(Fastspeed,5,3);
		Go_forward(Fastspeed,8,4);
		Back_pan(Fastspeed,9,5);
	}
}
