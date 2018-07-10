#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "adc.h"

void  rt_hw_adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);       //使能ADC1通道时钟


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    //PA01234 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       //模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


    ADC_Cmd(ADC1, ENABLE);  //使能指定的ADC1

    ADC_ResetCalibration(ADC1); //使能复位校准

    while (ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束

    ADC_StartCalibration(ADC1);  //开启AD校准

    while (ADC_GetCalibrationStatus(ADC1));  //等待校准结束

//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //使能指定的ADC1的软件转换启动功能

}
//获得ADC值
//ch:通道值 0~7
rt_uint16_t Get_Adc(rt_uint8_t ch)
{
    //设置指定ADC的规则组通道，一个序列，采样时间
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);   //ADC1,ADC通道,采样时间为239.5周期

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //使能指定的ADC1的软件转换启动功能

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束

    return ADC_GetConversionValue(ADC1);    //返回最近一次ADC1规则组的转换结果
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
    rt_kprintf("adc_Average:%d\n",temp_val / times);
    adcx=temp_val / times;

    temp=(float)adcx*(3.3/4096)*(8.4);
    adcx=temp;
    rt_kprintf("volt:%d.",adcx);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d\n",adcx);
    
    temp=(float)adcx*(3.3/4096);
    adcx=temp;
    rt_kprintf("volt:%d.",adcx);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d\n",adcx);
//		LCD_ShowxNum(172,150,temp,3,16,0X80);
    return temp_val / times;
}
FINSH_FUNCTION_EXPORT(adc_average, adc);


void rt_volt_thread_entry(void *parameter)
{
    rt_uint32_t temp_val = 0;
    rt_uint8_t t;
    float temp;
    rt_uint16_t adcx;
    while(1)
    {

    for (t = 0; t < 5; t++)
    {
        temp_val += Get_Adc(7);
        rt_thread_mdelay(50);
    }
    rt_kprintf("\n     I_adc_Average:%d\n",temp_val / 5);
    adcx=temp_val / 5;
//2A  1111
//1A  1208
//0A  1300
//-1A 1397
//-2A 1511
    temp=(float)adcx*(3.3/4096);
    adcx=temp;
    rt_kprintf("i_volt:%d.",adcx);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d\n",adcx);
//		LCD_ShowxNum(172,150,temp,3,16,0X80);
    adcx=0;
    temp_val=0;
    temp=0;
    
    for (t = 0; t < 5; t++)
    {
        temp_val += Get_Adc(6);
        rt_thread_mdelay(50);
    }
    rt_kprintf("\nadc_Average:%d\n",temp_val / 5);
    adcx=temp_val / 5;

    temp=(float)adcx*(3.3/4096)*(8.4);
    adcx=temp;
    rt_kprintf("bat_volt:%d.",adcx);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
    temp-=adcx;
    adcx=temp*1000;
    rt_kprintf("%d\n",adcx);
//		LCD_ShowxNum(172,150,temp,3,16,0X80);
    adcx=0;
    temp_val=0;
    temp=0;
        rt_thread_mdelay(500);
    }
}

int rt_volt_app_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("volt",
                           rt_volt_thread_entry, 0,
                           512, RT_THREAD_PRIORITY_MAX / 3 - 1, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);
    return 0;
}

INIT_APP_EXPORT(rt_volt_app_init);

