#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "adc.h"

void  rt_hw_adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //使能指定的ADC1的软件转换启动功能
}
INIT_BOARD_EXPORT(rt_hw_adc_init);

//获得ADC值
//ch:通道值 0~7
rt_uint16_t Get_Adc(rt_uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

u16 adc_average(rt_uint8_t ch, rt_uint8_t times)
{
    rt_uint32_t temp_val = 0;
    rt_uint8_t t;
    float temp;
    rt_uint16_t adcx;
    for (t = 0; t < times; t++)
    {
        temp_val += Get_Adc(ch);
        rt_thread_mdelay(50);
    }
    rt_kprintf("adc_average:%d\n", temp_val / times);
    adcx = temp_val / times;

    temp = (float)adcx * (3.3 / 4096);
    adcx = temp;
    rt_kprintf("volt:%d.", adcx);
    temp -= adcx;
    adcx = temp * 1000;
    rt_kprintf("%d | ", adcx);

    adcx = temp_val / times;
    temp = (float)adcx * (3.3 / 4096) * (8.4);
    adcx = temp;
    rt_kprintf("%d.", adcx);
    temp -= adcx;
    adcx = temp * 1000;
    rt_kprintf("%d\n", adcx);

    return temp_val / times;
}
FINSH_FUNCTION_EXPORT(adc_average, example: adc_average(0, 5) | 0->select adc channel; 5->adc acquisition times .);
