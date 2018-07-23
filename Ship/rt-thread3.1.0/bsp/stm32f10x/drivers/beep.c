#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "beep.h"

#define motor_rcc                   RCC_APB2Periph_GPIOA
#define beep_gpio_port              GPIOA
#define beepPin                     GPIO_Pin_12

void  rt_hw_beep_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(motor_rcc,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = beepPin;
    GPIO_Init(beep_gpio_port, &GPIO_InitStructure);
    
    GPIO_ResetBits(beep_gpio_port, beepPin);
}
INIT_BOARD_EXPORT(rt_hw_beep_init);

void beep(rt_uint32_t ms,rt_uint16_t count)
{
    for(rt_uint16_t i=0;i<count;i++)
    {
        GPIO_SetBits(beep_gpio_port, beepPin);
        rt_thread_mdelay(ms);
        GPIO_ResetBits(beep_gpio_port, beepPin);
        rt_thread_mdelay(ms);
    }
}
