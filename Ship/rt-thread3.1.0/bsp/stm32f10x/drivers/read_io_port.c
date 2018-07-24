#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "read_io_port.h"

void  rt_hw_portDEFG_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}
INIT_BOARD_EXPORT(rt_hw_portDEFG_init);

rt_uint16_t readPort_D()
{
    return GPIO_ReadInputData(GPIOD);
}

rt_uint16_t readPort_E()
{
    return GPIO_ReadInputData(GPIOE);
}

rt_uint16_t readPort_F()
{
    return GPIO_ReadInputData(GPIOF);
}

rt_uint16_t readPort_G()
{
    return GPIO_ReadInputData(GPIOG);
}
