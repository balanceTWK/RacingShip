#include <rthw.h>
#include <rtthread.h>
#include "control_app.h"
#include "inv_mpu.h"

rt_uint16_t debugControl=0;
void control_thread_entry(void *parameter)
{
    float pitch, roll, yaw;           //欧拉角
    while (1)
    {
        if(debugControl==0)
        {
            rt_thread_delay(rt_tick_from_millisecond(200));
        }
        else
        {
            if (mpu_mpl_get_data(&pitch, &roll, &yaw) == 0)
            {
                rt_kprintf("mpu9250 get times : %d, pitch:%d, roll:%d, yaw:%d \n",debugControl,(rt_int16_t)(pitch*100), (rt_int16_t)(roll*100), (rt_int16_t)(yaw*100));
            }
            else
            {
                rt_kprintf("mpu9250 miss times : %d\n",debugControl);
            }
            debugControl++;
            if(debugControl>1000)
            {
                debugControl=0;
            }
            rt_thread_delay(rt_tick_from_millisecond(20));
        }
    }
}

rt_err_t debug_Control()
{
    if(debugControl==0)
    {
        debugControl=1;
    }
    return 0;
}
MSH_CMD_EXPORT(debug_Control,debugControl);

int control_app_init(void)
{
    rt_thread_t tid;
    
    /* 创建master线程 */
    tid = rt_thread_create("control",
                           control_thread_entry,
                           RT_NULL,
                           2048,
                           5,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}
