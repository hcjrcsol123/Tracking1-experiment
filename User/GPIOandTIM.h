#ifndef _GPIOandTIM_H
#define _GPIOandTIM_H

#define Forward_direction 0//正转
#define Reverse 1//反转
void TIM1_PWM_Init(void);
void Tracing1_GPIO_Init(void);
void electric_machinery1_PWM(uint16_t Compare1);//电机1转速调节
void electric_machinery2_PWM(uint16_t Compare2);//电机2转速调节
void electric_machinery3_PWM(uint16_t Compare3);//电机3转速调节
void electric_machinery4_PWM(uint16_t Compare3);//电机4转速调节
void electric_machinery_PWM(uint16_t Compare);//电机总转速调节
void electric_machinery_direction_control(uint8_t motor1,uint8_t motor2,uint8_t motor3,uint8_t motor4);//电机转向调节
#endif
