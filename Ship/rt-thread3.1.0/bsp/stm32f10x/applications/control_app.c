#include <rthw.h>
#include <rtthread.h>
#include "control_app.h"
#include "steeringengine.h"
#include "control_function.h"
#include "inv_mpu.h"
#include "motor.h"

#include "beep.h"
#define DebugBeep beep(100,2);

volatile rt_uint16_t debugControl = 0;
void control_thread_entry(void *parameter)//根据反馈,控制方向和速度.
{
    float pitch, roll, yaw;           //欧拉角
    rt_uint32_t feedback;

#if 1
    steeringEngine_1(500);
    motor1_control(1, 4000);
    DebugBeep
    steeringEngine_1(750);
    motor1_control(1, 5000);
    DebugBeep
    steeringEngine_1(1000);
    motor1_control(1, 6000);
    DebugBeep
    rt_thread_delay(rt_tick_from_millisecond(500));
    steeringEngine_1(750);
    rt_thread_delay(rt_tick_from_millisecond(500));
    DebugBeep
    motor1_control(1, 0);
#endif

    while (1)
    {
        if (debugControl == 0)
        {
            rt_thread_delay(rt_tick_from_millisecond(200));
        }
        else if(debugControl == 1)
        {
            feedback = get_9infraredProbe_offset();
            if (feedback != 0)
            {
                if ((feedback & 0xC0000000) == 0x80000000) //判断为3个红外探头的时候
                {
                    feedback = (feedback & 0x0000FFFF);
                    steeringEngine_1(feedback * 16 + 484); //舵机只有两个可能的值 650 和 850
                    motor1_control(1, 4000);
                }
                else if ((feedback & 0xC0000000) == 0xC0000000) //判断为6个探头的时候
                {
                    feedback = (feedback & 0x0000FFFF);
                    steeringEngine_1(feedback * 40 + 560); //舵机半范围调节,500-1000
                    motor1_control(1, 5500);
                }
                else if ((feedback & 0x0000FFFF) == 0x0000FFFF)
                {
                    steeringEngine_1(750);//舵机中间值
                }
            }
            else
            {
                feedback = get_26infraredProbe_offset(); //判断为26个探头的时候
                if (feedback != 0)
                {
                    if(feedback >= 23)
                    {
                        feedback = 26;
                    }
                    else if(feedback <= 4)
                    {
                        feedback = 1;
                    }                        
                    steeringEngine_1(feedback * 20 + 480); //舵机全范围调节,250-1250
                    motor1_control(1, 7000);
                }
            }
        }
        else if(debugControl >= 2)
        {
            if((mpu_mpl_get_data(&pitch,&roll,&yaw)==0))
            {
                rt_kprintf("pitch=%d, roll=%d, yaw=%d  times=%d\n",(int)(pitch*10),(int)(roll*10),(int)(yaw*10),debugControl-1);
            }
            else
            {
                rt_kprintf("mpu9250 err !  times=%d\n",debugControl-1);
            }
            debugControl++;
            if(debugControl>=1002)
            {
                debugControl = 0;
            }
        }
        rt_thread_delay(rt_tick_from_millisecond(10));
    }
}

void debug(int argc, char **argv)
{
    if (argc == 2)
    {
        if (!rt_strcmp(argv[1], "0"))
        {
            debugControl = 0;
            motor1_control(1, 0);
            motor2_control(1, 0);
            steeringEngine_1(750);
            rt_kprintf("\ndebugControl=0\n");
        }
        else if (!rt_strcmp(argv[1], "1"))
        {
            debugControl = 1;
            rt_kprintf("\ndebugControl=1\n");
        }
        else if (!rt_strcmp(argv[1], "2"))
        {
            debugControl = 2;
            motor1_control(1, 0);
            motor2_control(1, 0);
            steeringEngine_1(750);
            rt_kprintf("\ndebugControl=2\n");
        }
        else
        {
            rt_kprintf("err code !\n");
        }
    }
    else
    {
        rt_kprintf("\n err command ! example : debug 0 \n");
    }
}
MSH_CMD_EXPORT(debug, debug mode. (0 1 2 ...));

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
