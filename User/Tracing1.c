#include "stm32f10x.h"                  // Device header
#include "Tracing1.h"
#include "GPIOandTIM.h"
#include "USART2.h"
#include "Delay.h"
//F2=A GPIO_Pin_0      F4=A GPIO_Pin_1 前 F
//B2=A GPIO_Pin_4      B4=A GPIO_Pin_5 后 B
//L2=A GPIO_Pin_6      L4=A GPIO_Pin_7 左 L
//R2=B GPIO_Pin_5      R4=B GPIO_Pin_6 右 R
//---------------------------------
//electric_machinery1=左前轮
//electric_machinery2=右前轮
//electric_machinery3=左后轮
//electric_machinery4=右后轮
//---------------------------------



void In_situ_direction_correction(uint8_t Action_code)//原地方向修正，参数为平移动作标志
{
	// uint16_t maxsize = 0;
	// uint16_t maxsize_size = 0;
	if(Action_code==forward)//前平移动作完成
	{
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|
		   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))//F2|F4|B2|B4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))
			{//方向右偏,F2|B4低电平
				electric_machinery_direction_control(Reverse,Forward_direction,Reverse,Forward_direction);//左转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET))
			{//方向左偏,F4|B2低电平
				electric_machinery_direction_control(Forward_direction,Reverse,Forward_direction,Reverse);//右转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
		}
		
				electric_machinery_PWM(0);

		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|
			 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))//L2|L4|R2|R4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))
			{//位置偏后L4|R4低电平
				electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
				//调整为正转
				electric_machinery_PWM(Position_fine_adjustment);//向前微调位置
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET))
			{//位置偏前L2|R2低电平
				electric_machinery_direction_control(Reverse,Reverse,Reverse,Reverse);
				//调整为反转
				electric_machinery_PWM(Position_fine_adjustment);//向后微调位置
			}
		}
		electric_machinery_PWM(0);//调整完毕，停止。
		electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
		//方向回正。
	}
	
	
	
	if(Action_code==Turn_left)//左平移动作完成
	{
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|
		   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))//L2|L4|R2|R4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))
			{//方向右偏,L2|R4低电平
				electric_machinery_direction_control(Reverse,Forward_direction,Reverse,Forward_direction);//左转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET))
			{//方向左偏,L4|R2低电平
				electric_machinery_direction_control(Forward_direction,Reverse,Forward_direction,Reverse);//右转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
		}
		
		electric_machinery_PWM(0);

		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|
			 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))//F2|F4|B2|B4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET))
			{//位置偏后F2|B2低电平（以左方向为正方向）
				electric_machinery_direction_control(Forward_direction,Reverse,Reverse,Forward_direction);
				//调整为左前轮、右后轮正转，右前轮，左后轮反转
				electric_machinery_PWM(Position_fine_adjustment);//向前微调位置
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))
			{//位置偏前F4|B4低电平（以左方向为正方向）
				electric_machinery_direction_control(Reverse,Reverse,Reverse,Reverse);
				//调整为右前轮、左后轮正转、左前轮、右后轮反转
				electric_machinery_PWM(Position_fine_adjustment);//向后微调位置
			}
		}
		electric_machinery_PWM(0);//调整完毕，停止。
		electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
		//方向回正。
	}
	
	
	if(Action_code==Turn_right)//右平移动作完成
	{
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|
		   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))//R2|R4|L2|L4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))
			{//方向右偏,R4|L2低电平
				electric_machinery_direction_control(Reverse,Forward_direction,Reverse,Forward_direction);//左转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET))
			{//方向左偏,R2|L4低电平
				electric_machinery_direction_control(Forward_direction,Reverse,Forward_direction,Reverse);//右转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
		}
		electric_machinery_PWM(0);
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|
			 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))//F2|F4|B2|B4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))
			{//位置偏后F4|B4低电平（以右方向为正方向）
				electric_machinery_direction_control(Reverse,Forward_direction,Forward_direction,Reverse);
				//调整为左前轮、右后轮反转，右前轮，左后轮正转。
				electric_machinery_PWM(Position_fine_adjustment);//向前微调位置
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET))
			{//位置偏前F2|B2低电平
				electric_machinery_direction_control(Forward_direction,Reverse,Reverse,Forward_direction);
				//调整为左前轮、右后轮正转、右前轮、左后轮反转
				electric_machinery_PWM(Position_fine_adjustment);//向后微调位置
			}
		}
		electric_machinery_PWM(0);//调整完毕，停止。
		electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
		//方向回正。
	}
	
	if(Action_code==back)//后平移动作完成
	{
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET)|
		   (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET))//B2|B4|F2|F4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET))
			{//方向右偏,B4|F2低电平
				electric_machinery_direction_control(Reverse,Forward_direction,Reverse,Forward_direction);//左转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET))
			{//方向左偏,B2|F4低电平
				electric_machinery_direction_control(Forward_direction,Reverse,Forward_direction,Reverse);//右转
				electric_machinery_PWM(In_situ_rotation_speed);//调整原地旋转时电机转速
			}
		}
		
		electric_machinery_PWM(0);
		
		while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)|
			 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))//L2|L4|R2|R4低电平
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)|(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET))
			{//位置偏后L2|R2低电平（以后方向为正方向）
				electric_machinery_direction_control(Reverse,Reverse,Reverse,Reverse);
				//调整为正转
				electric_machinery_PWM(Position_fine_adjustment);//向前微调位置
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)|(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET))
			{//位置偏前L4|R4低电平（以后方向为正方向）
				electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
				//调整为反转
				electric_machinery_PWM(Position_fine_adjustment);//向后微调位置
			}
		}
		electric_machinery_PWM(0);//调整完毕，停止。
		electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);
		//方向回正。
	}
}

void Go_forward(uint16_t PWMparameter,uint16_t count,uint8_t step)//向前平移，输入电机转速、前进格数、步骤序号
{
	if(RX_data==step)
	{
		RX_data = 0;//清除数据
		uint8_t i = 0;
		uint8_t count1 = 0;
		uint8_t A = 0;
		uint16_t speed = PWMparameter;
		electric_machinery_direction_control(Forward_direction,Forward_direction,Forward_direction,Forward_direction);//所有电机正转
		electric_machinery_PWM(speed);
		
		while(count1<=count)
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)&&(i==0))//L2或R2低电平
			{
				count1++;//走过一个格子
				i =1;//用于防止扫描过快导致的重复计数
				
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==RESET) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==RESET))
			{
				i = 0;
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET) | (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET))//F2|F4低电平
			{		
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET)//车右偏
				{
					electric_machinery1_PWM(speed-Correction_parameters);
					electric_machinery4_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery2_PWM(speed);
					electric_machinery3_PWM(speed);
					//向左移动
				}
				else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)//车左偏
				{
					electric_machinery2_PWM(speed-Correction_parameters);
					electric_machinery3_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery1_PWM(speed);
					electric_machinery4_PWM(speed);

					//向右移动
				}
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==RESET))//F2&&F4高电平
			{
				electric_machinery_PWM(speed);//将转速调回一致。
			}
			if((count1==count-1)&&(A==0))//到达最后一格的前一格，进行减速。
			{
				speed = Lowspeed;A = 1;
				electric_machinery_PWM(Lowspeed);
			}
		}
		electric_machinery_direction_control(Reverse,Reverse,Reverse,Reverse);//刹车
		Delay_ms(100);//刹车延时
		electric_machinery_PWM(0);//移动完指定格数，停止
		Delay_ms(1000);//刹车延迟
		In_situ_direction_correction(forward);//原地方向位置修正
		USART2_SEND_data(Pan_Over);//发送完成标志
	}
	
}


void Pan_Left(uint16_t PWMparameter,uint16_t count,uint8_t step)//向左平移，以左方向为正方向，输入电机转速、平移格数、步骤序号
{
	if(RX_data==step)
	{
		RX_data = 0;
		uint8_t i = 0;
		uint8_t count1 = 0;
		uint8_t A=0;
		uint16_t speed = PWMparameter;
		electric_machinery_direction_control(Reverse,Forward_direction,Forward_direction,Reverse);//左前轮、右后轮正转，右前轮，左后轮反转
		electric_machinery_PWM(speed);
		while(count1<=count)
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET)&&(i==0))//B4&&F4低电平
			{
				count1++;//走过一个格子
				i=1;//防止重复计数
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==RESET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==RESET))
			{
				i=0;
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET) | (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET))//L2|L4低电平
			{		//以左边为正方向
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==SET)//车右偏
				{
					electric_machinery1_PWM(speed);
					electric_machinery4_PWM(speed);
					electric_machinery2_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery3_PWM(speed-Correction_parameters);
					//向左移动
				}
				else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET)//车左偏
				{
					electric_machinery2_PWM(speed);
					electric_machinery3_PWM(speed);
					electric_machinery1_PWM(speed-Correction_parameters);
					electric_machinery4_PWM(speed);//撤除参数-Correction_parameters
					//向右移动
				}
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==RESET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==RESET))//车左右灰度传感器高电平，车辆摆正。
			{
				electric_machinery_PWM(speed);//将转速调整一致。
			}
			if((count1==count-1)&&(A==0))
			{
				speed = Lowspeed;A = 1;
				electric_machinery_PWM(Lowspeed);
			}
		}
		electric_machinery_direction_control(Forward_direction,Reverse,Reverse,Forward_direction);//刹车
		Delay_ms(100);//刹车延迟
		electric_machinery_PWM(0);//移动完指定格数，停止
		Delay_ms(1000);//刹车延迟
		In_situ_direction_correction(Turn_left);//原地方向位置修正
		USART2_SEND_data(Pan_Over);
	}
	
}

void Pan_right(uint16_t PWMparameter,uint16_t count,uint8_t step)//向右平移，以右方向为正方向,输入电机转速、前进格数、步骤序号
{
	if(RX_data==step)
	{
		RX_data=0;
		uint8_t count1 = 0;
		uint8_t i = 0;
		uint8_t A=0;
		uint16_t speed = PWMparameter;
		electric_machinery_direction_control(Forward_direction,Reverse,Reverse,Forward_direction);//又前轮、左后轮正转，左前轮，又后轮反转
		electric_machinery_PWM(speed);
		while(count1<=count)
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==SET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)&&(i==0))//F2或B2低电平
			{
				count1++;//走过一个格子
				i=1;
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==RESET))
			{
				i=0;
			}
			if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET) | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET))//R2|R4低电平
			{		//以右边为正方向
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET)//车右偏
				{
					electric_machinery1_PWM(speed);
					electric_machinery4_PWM(speed);
					electric_machinery2_PWM(speed-Correction_parameters);
					electric_machinery3_PWM(speed);//撤除参数-Correction_parameters

					//向左移动
				}
				else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==SET)//车左偏
				{
					electric_machinery2_PWM(speed);
					electric_machinery3_PWM(speed);
					electric_machinery1_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery4_PWM(speed-Correction_parameters);
					//向右移动
				}
			}
			if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==RESET) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==RESET))//R2&&R4器高电平，车辆摆正。
			{
				electric_machinery_PWM(speed);//将转速调整一致。
			}
			if((count1==count-1)&&(A==0))
			{
				speed = Lowspeed;A = 1;
				electric_machinery_PWM(Lowspeed);
			}
		}
		electric_machinery_PWM(0);//移动完指定格数，停止
		Delay_ms(1000);
		In_situ_direction_correction(Turn_right);//原地方向位置修正
		USART2_SEND_data(Pan_Over);
	}

}


void Back_pan(uint16_t PWMparameter,uint16_t count,uint8_t step)//向后平移，以后方向为正方向
{
	if(RX_data==step)
	{
		RX_data=0;
		uint8_t count1 = 0;
		uint8_t i=0;
		uint8_t A=0;
		uint16_t speed = PWMparameter;
		electric_machinery_direction_control(Reverse,Reverse,Reverse,Reverse);//所有电机反转
		electric_machinery_PWM(speed);
		while(count1<=count)
		{
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==SET) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==SET)&&(i==0))//L4或R4低电平
			{
				count1++;//走过一个格子
				i=1;
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==RESET) && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==RESET))
			{
				i=0;
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET) | (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET))//B2|B4低电平
			{		//以后边为正方向
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==SET)//车右偏
				{
					electric_machinery1_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery4_PWM(speed-Correction_parameters);
					electric_machinery2_PWM(speed);
					electric_machinery3_PWM(speed);
					//向左移动
				}
				else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==SET)//车左偏
				{
					electric_machinery2_PWM(speed);//撤除参数-Correction_parameters
					electric_machinery3_PWM(speed-Correction_parameters);
					electric_machinery1_PWM(speed);
					electric_machinery4_PWM(speed);
					//向右移动
				}
			}
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==RESET) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==RESET))//R2&&B4器高电平，车辆摆正。
			{
				electric_machinery_PWM(speed);//将转速调整一致。
			}
			if((count1==count-1)&&(A==0))
			{
				speed = Lowspeed;A = 1;
				electric_machinery_PWM(Lowspeed);
			}
		}
		electric_machinery_PWM(0);//移动完指定格数，停止
		Delay_ms(1000);
		In_situ_direction_correction(back);//原地方向位置修正
		USART2_SEND_data(Pan_Over);
	}
}
