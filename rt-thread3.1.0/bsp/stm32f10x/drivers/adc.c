#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "adc.h"

void  rt_hw_adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);       //ʹ��ADC1ͨ��ʱ��


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    //PA01234 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       //ģ����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


    ADC_Cmd(ADC1, ENABLE);  //ʹ��ָ����ADC1

    ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼

    while (ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����

    ADC_StartCalibration(ADC1);  //����ADУ׼

    while (ADC_GetCalibrationStatus(ADC1));  //�ȴ�У׼����

//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //ʹ��ָ����ADC1�����ת����������

}
//���ADCֵ
//ch:ͨ��ֵ 0~7
rt_uint16_t Get_Adc(rt_uint8_t ch)
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);   //ADC1,ADCͨ��,����ʱ��Ϊ239.5����

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //ʹ��ָ����ADC1�����ת����������

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//�ȴ�ת������

    return ADC_GetConversionValue(ADC1);    //�������һ��ADC1�������ת�����
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
    rt_kprintf("adc_average:%d\n",temp_val / times);
    adcx=temp_val / times;

    temp=(float)adcx*(3.3/4096)*(8.4);
    adcx=temp;
    rt_kprintf("volt:%d.",adcx);
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d | ",adcx);
    
    temp=(float)adcx*(3.3/4096);
    adcx=temp;
    rt_kprintf("%d.",adcx);
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d\n",adcx);
    return temp_val / times;
}
FINSH_FUNCTION_EXPORT(adc_average, example: adc_average(0,5) | 0->select adc channel;5->adc acquisition times .);
