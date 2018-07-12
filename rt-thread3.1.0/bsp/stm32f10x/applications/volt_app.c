#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "adc.h"
#include "wireless_app.h"

rt_bool_t DebugFlag=0;

void rt_volt_thread_entry(void *parameter)
{
    rt_uint32_t temp_val = 0;
    rt_uint8_t t;
    float temp;
    rt_uint16_t adcx;
    rt_int16_t current;
    while(1)
    {
    if(DebugFlag)
    {
        for (t = 0; t < 5; t++)
        {
            temp_val += Get_Adc(7);
            rt_thread_mdelay(50);
        }
        adcx=temp_val / 5;
        rt_kprintf("\ncurrent_adc_Average:%d\n",adcx);
        current=adcx-1300;
        if(current<0)
        {
            rt_kprintf("current:\033[0;31m-");
        }
        else
        {
            rt_kprintf("current:\033[0;31m");
        }
        current=current/100;
        current=__fabs(current);
        rt_kprintf("%d.",current);
        current=adcx-1300;
        current=current%100;
        current=current/10;
        current=__fabs(current);
        rt_kprintf("%d",current);
        current=adcx-1300;
        current=current%10;
        current=__fabs(current);
        rt_kprintf("%d\033[0m\n",current);
        
    // 2A   1111
    // 1A   1208
    // 0A   1300
    //-1A   1397
    //-2A   1511

        current=0;
        adcx=0;
        temp_val=0;
        temp=0;
        
        for (t = 0; t < 5; t++)
        {
            temp_val += Get_Adc(6);
            rt_thread_mdelay(10);
        }
        rt_kprintf("\nbat_adc_Average:%d\n",temp_val / 5);
        adcx=temp_val / 5;

        temp=(float)adcx*(3.3/4096)*(8.4);
        adcx=temp;
        rt_kprintf("bat_volt:\033[36m%d.",adcx);
        temp-=adcx;
        adcx=temp*10;
        rt_kprintf("%d\033[0m\n",adcx);
        adcx=0;
        temp_val=0;
        temp=0;
        rt_thread_mdelay(10);
    }
    else
    {
        wireless_putchar(0x5a);
        wireless_putchar(0xa5);
        for (t = 0; t < 5; t++)
        {
            temp_val += Get_Adc(7);
            rt_thread_mdelay(10);
        }
        adcx=temp_val / 5;
        current=adcx-1300;
        wireless_putchar((rt_uint8_t)(current>>8));
        wireless_putchar((rt_uint8_t)current);

        temp_val=0;
        
        for (t = 0; t < 5; t++)
        {
            temp_val += Get_Adc(6);
            rt_thread_mdelay(10);
        }
        adcx=temp_val / 5;

        temp=(float)adcx*(3.3/4096)*(8.4);
        adcx=temp*10;
        wireless_putchar((rt_uint8_t)adcx>>8);
        wireless_putchar((rt_uint8_t)adcx);

        current=current+adcx;
        wireless_putchar((rt_uint8_t)current>>8);
        wireless_putchar((rt_uint8_t)current);
        
        temp_val=0;
        
//        rt_thread_mdelay(5);
    }

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

//INIT_APP_EXPORT(rt_volt_app_init);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void master_thread_entry(void *parameter)
{
    rt_thread_t tid;
    /* 打开串口 */
    if (wireless_open("uart3") != RT_EOK)
    {
        rt_kprintf("uart3 open error.\n");
    }
    wireless_putstring(" wireless is working.\n");
	rt_thread_delay(rt_tick_from_millisecond(50));
    tid = rt_thread_create("volt",
                           rt_volt_thread_entry, 0,
                           512, RT_THREAD_PRIORITY_MAX / 3 - 1, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);
}

int master_init(void)
{
    rt_thread_t tid;

    /* 创建test线程 *////////////////////////////////////////
    tid = rt_thread_create("master",
                           master_thread_entry,
                           RT_NULL,
                           256,
                           5,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);	
    
    return 0;
}
INIT_APP_EXPORT(master_init);

void debug()
{
    DebugFlag=~DebugFlag;
}
FINSH_FUNCTION_EXPORT(debug,start debug output!);
