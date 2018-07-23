#ifndef __BEEP_H
#define __BEEP_H	
#include <rthw.h>
#include <rtthread.h>
void  rt_hw_beep_init(void);
void beep(rt_uint32_t ms,rt_uint16_t count);
#endif 
