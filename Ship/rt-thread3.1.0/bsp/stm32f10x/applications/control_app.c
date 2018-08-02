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
void control_thread_entry(void *parameter)//���ݷ���,���Ʒ�����ٶ�.
{
    float pitch, roll, yaw;           //ŷ����
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
                if ((feedback & 0xC0000000) == 0x80000000) //�ж�Ϊ3������̽ͷ��ʱ��
                {
                    feedback = (feedback & 0x0000FFFF);
                    steeringEngine_1(feedback * 16 + 484); //���ֻ���������ܵ�ֵ 650 �� 850
                    motor1_control(1, 4000);
                }
                else if ((feedback & 0xC0000000) == 0xC0000000) //�ж�Ϊ6��̽ͷ��ʱ��
                {
                    feedback = (feedback & 0x0000FFFF);
                    steeringEngine_1(feedback * 40 + 560); //����뷶Χ����,500-1000
                    motor1_control(1, 5500);
                }
                else if ((feedback & 0x0000FFFF) == 0x0000FFFF)
                {
                    steeringEngine_1(750);//����м�ֵ
                }
            }
            else
            {
                feedback = get_26infraredProbe_offset(); //�ж�Ϊ26��̽ͷ��ʱ��
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
                    steeringEngine_1(feedback * 20 + 480); //���ȫ��Χ����,250-1250
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

    /* ����master�߳� */
    tid = rt_thread_create("control",
                           control_thread_entry,
                           RT_NULL,
                           2048,
                           5,
                           10);
    /* �����ɹ��������߳� */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}
