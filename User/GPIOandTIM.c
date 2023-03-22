#include "stm32f10x.h"                  // Device header
#include "GPIOandTIM.h"


void Tracing1_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入，用于接收灰度传感器电平
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出，用于控制电机正反转
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}



void TIM1_PWM_Init(void)//使用TIM1作为PWM输出
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//tim1的CH1-4
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100;   //未设置
	TIM_TimeBaseInitStruct.TIM_Prescaler = 36-1;   //未设置
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//未设置
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0xFFFF;//未设置
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC2Init(TIM1,&TIM_OCInitStruct);
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	TIM_Cmd(TIM1,ENABLE);
}


void electric_machinery1_PWM(uint16_t Compare1)//电机1占空比输出设置。
{

	TIM_SetCompare1(TIM1,Compare1+5);
}

void electric_machinery2_PWM(uint16_t Compare2)//电机2占空比输出设置。
{
	TIM_SetCompare2(TIM1,Compare2);
}

void electric_machinery3_PWM(uint16_t Compare3)//电机3占空比输出设置。
{ 
	TIM_SetCompare3(TIM1,Compare3);
}

void electric_machinery4_PWM(uint16_t Compare4)//电机4占空比输出设置。
{
	TIM_SetCompare4(TIM1,Compare4);
}

void electric_machinery_PWM(uint16_t Compare)
{
	TIM_SetCompare1(TIM1,Compare);
	TIM_SetCompare2(TIM1,Compare);
	TIM_SetCompare3(TIM1,Compare);
	TIM_SetCompare4(TIM1,Compare);
}


//转向----------------------------------
//PB0&&PB1控制电机1
//PB7&&PB10控制电机2
//PB11&&PB12控制电机3
//PB13&&PB14控制电机4
void electric_machinery_direction_control(uint8_t motor1,uint8_t motor2,uint8_t motor3,uint8_t motor4)//电机转向调节，0正转，1反转
{
	if(motor1==Forward_direction)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	}
	else if(motor1==Reverse)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
	
	if(motor2==Forward_direction)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	}
	else if(motor2==Reverse)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	}
	
	if(motor3==Forward_direction)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
	else if(motor3==Reverse)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
	
	if(motor4==Forward_direction)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	}
	else if(motor4==Reverse)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
}

