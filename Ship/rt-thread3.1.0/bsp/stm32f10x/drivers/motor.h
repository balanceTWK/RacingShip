#ifndef __MOTOR_H
#define __MOTOR_H	
#include <rthw.h>
#include <rtthread.h>
void  rt_hw_motor_init(void);
void  motor1_control(rt_uint8_t direction,rt_uint16_t pwmCompare);
void  motor2_control(rt_uint8_t direction,rt_uint16_t pwmCompare);
#endif 
