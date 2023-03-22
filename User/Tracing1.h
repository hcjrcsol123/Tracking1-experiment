#ifndef _Tracing1_H
#define _Tracing1_H
#include "stm32f10x.h"// Device header
#define In_situ_rotation_speed 60//原地旋转电机PWM占空比
#define Fastspeed 75//最高速度
#define Lowspeed 70
#define Position_fine_adjustment 10//小幅度位移时电机转速
#define Correction_parameters 10 //平移轨迹修正参数
#define forward 1//前平移标志
#define Turn_left 2//左移标志
#define Turn_right 3//右平移标志
#define back 4//后平移标志
#define Pan_Over 0xFF//单次平移行为完成
void In_situ_direction_correction(uint8_t Action_code);//原地方向修正
void Go_forward(uint16_t PWMparameter,uint16_t count,uint8_t step);//向前平移，输入电机转速、前进格数、步骤序号
void Pan_Left(uint16_t PWMparameter,uint16_t count,uint8_t step);//向左平移，以左方向为正方向，输入电机转速、平移格数
void Pan_right(uint16_t PWMparameter,uint16_t count,uint8_t step);//向右平移，以右方向为正方向,输入电机转速、前进格数、步骤序号
void Back_pan(uint16_t PWMparameter,uint16_t count,uint8_t step);//向后平移，以后方向为正方向
#endif
