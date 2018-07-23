#ifndef __PWM_H
#define __PWM_H	
#include <rthw.h>
#include <rtthread.h>
void  rt_hw_pwm_init(void);
void pwm(rt_uint8_t ch, rt_uint16_t compare);
#endif 
