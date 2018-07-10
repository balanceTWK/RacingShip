#ifndef __ADC_H
#define __ADC_H	
#include <rthw.h>
#include <rtthread.h>
void rt_hw_adc_init(void);
rt_uint16_t  Get_Adc(rt_uint8_t ch); 
rt_uint16_t adc_Average(rt_uint8_t ch,rt_uint8_t times); 
 
#endif 
