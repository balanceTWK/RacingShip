#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "pwm.h"
#include "motor.h"

#define motor_rcc                   RCC_APB2Periph_GPIOB
#define motor_gpio_port             GPIOB
#define motor1_in1                  GPIO_Pin_12
#define motor1_in2                  GPIO_Pin_13
#define motor2_in1                  GPIO_Pin_14
#define motor2_in2                  GPIO_Pin_15

void  rt_hw_motor_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(motor_rcc,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = motor1_in1|motor1_in2|motor2_in1|motor2_in2;
    GPIO_Init(motor_gpio_port, &GPIO_InitStructure);
    
    GPIO_ResetBits(motor_gpio_port, motor1_in1|motor1_in2|motor2_in1|motor2_in2);
}
INIT_BOARD_EXPORT(rt_hw_motor_init);

void  motor1_control(rt_uint8_t direction,rt_uint16_t pwmCompare)
{
    if(direction)
    {
        GPIO_ResetBits(motor_gpio_port, motor1_in1);
        GPIO_SetBits(motor_gpio_port, motor1_in2);
    }
    else
    {
        GPIO_ResetBits(motor_gpio_port, motor1_in2);
        GPIO_SetBits(motor_gpio_port, motor1_in1);
    }
    pwm(12,pwmCompare);
}

void  motor2_control(rt_uint8_t direction,rt_uint16_t pwmCompare)
{
    if(direction)
    {
        GPIO_ResetBits(motor_gpio_port, motor2_in1);
        GPIO_SetBits(motor_gpio_port, motor2_in2);
    }
    else
    {
        GPIO_ResetBits(motor_gpio_port, motor2_in2);
        GPIO_SetBits(motor_gpio_port, motor2_in1);
    }
    pwm(11,pwmCompare);
}

FINSH_FUNCTION_EXPORT(motor1_control, example: motor1_control(1, 4000));
FINSH_FUNCTION_EXPORT(motor2_control, example: motor2_control(1, 4000));
