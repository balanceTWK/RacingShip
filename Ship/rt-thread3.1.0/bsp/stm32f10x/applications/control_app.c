#include <rthw.h>
#include <rtthread.h>
#include "control_app.h"
#include "steeringengine.h"
#include "control_function.h"

rt_uint16_t debugControl=0;
void control_thread_entry(void *parameter)//根据反馈,控制方向和速度.
{
    float pitch, roll, yaw;           //欧拉角
    rt_uint32_t feedback;
    while (1)
    {
        if(debugControl==0)
        {
            rt_thread_delay(rt_tick_from_millisecond(200));
        }
        else
        {
            feedback=get_infraredProbe_offset();
            if(feedback!=0)
            {
                steeringEngine_1(feedback*40+210);
            }
            rt_kprintf("control thread times : %d\n",debugControl);
            debugControl++;
            if(debugControl>1000)
            {
                debugControl=0;
            }
            rt_thread_delay(rt_tick_from_millisecond(10));
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
