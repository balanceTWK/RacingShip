#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "pwm.h"
//9999,143
void stm32_tim1_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 143;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);

    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare4(TIM1, 0);
}

void stm32_tim3_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure = {0};

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 143;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);

    TIM_SetCompare1(TIM3, 0);
    TIM_SetCompare2(TIM3, 0);
    TIM_SetCompare3(TIM3, 0);
    TIM_SetCompare4(TIM3, 0);
}
void  rt_hw_pwm_init(void)
{
    stm32_tim1_init();
    stm32_tim3_init();
}
void pwm(rt_uint8_t ch, rt_uint16_t compare)
{
    if (compare > 9999)
    {
        rt_kprintf("The range of the second parameter is between 0-9999 .");
    }
    else
    {
        switch (ch)
        {
        case 1:
            TIM_SetCompare1(TIM3, compare);
            break;
        case 2:
            TIM_SetCompare2(TIM3, compare);
            break;
        case 3:
            TIM_SetCompare3(TIM3, compare);
            break;
        case 4:
            TIM_SetCompare4(TIM3, compare);
            break;
        case 5:
            TIM_SetCompare1(TIM1, compare);
            break;
        case 6:
            TIM_SetCompare2(TIM1, compare);
            break;
        case 7:
            TIM_SetCompare3(TIM1, compare);
            break;
        case 8:
            TIM_SetCompare4(TIM1, compare);
            break;
        default:
            rt_kprintf("The range of the first parameter is between 1-8 .");
            break;
        }
    }
}
FINSH_FUNCTION_EXPORT(pwm, example: pwm(1, 300));
