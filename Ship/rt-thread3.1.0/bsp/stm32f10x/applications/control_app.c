#include <rthw.h>
#include <rtthread.h>
#include "control_app.h"
#include "steeringengine.h"
#include "control_function.h"

rt_uint16_t debugControl=0;
void control_thread_entry(void *parameter)//���ݷ���,���Ʒ�����ٶ�.
{
    float pitch, roll, yaw;           //ŷ����
    rt_uint32_t feedback;
    while (1)
    {
        if(debugControl==0)
        {
            rt_thread_delay(rt_tick_from_millisecond(200));
        }
        else
        {
            feedback=get_9infraredProbe_offset();
            if(feedback!=0)
            {
                if((feedback&0xC0000000)==0x80000000)//�ж�Ϊ3������̽ͷ��ʱ��
                {
                    feedback=(feedback&0x0000FFFF);
                    steeringEngine_1(feedback*40+210);//���ֻ���������ܵ�ֵ 650 �� 850
                }
                else if((feedback&0xC0000000)==0xC0000000)//�ж�Ϊ6��̽ͷ��ʱ��
                {
                    feedback=(feedback&0x0000FFFF);
                    steeringEngine_1(feedback*100+400);//����뷶Χ����,500-1000
                }
                else if((feedback&0x0000FFFF)==0x0000FFFF)
                {
                    steeringEngine_1(750);//����м�ֵ
                }
            }
            else
            {
                feedback=get_26infraredProbe_offset();//�ж�Ϊ26��̽ͷ��ʱ��
                if(feedback!=0)
                {
                    steeringEngine_1(feedback*40+210);//���ȫ��Χ����,250-1250
                }
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
