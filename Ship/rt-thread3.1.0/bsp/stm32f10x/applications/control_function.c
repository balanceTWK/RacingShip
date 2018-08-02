#include <rthw.h>
#include <rtthread.h>
#include "inv_mpu.h"
#include "motor.h"
#include "read_io_port.h"
#include "control_function.h"

static rt_uint32_t frontProbe = 0,closeProbe = 0, sum = 0;
static rt_uint16_t port_D = 0, port_E = 0, port_F = 0;

static rt_uint8_t index, count;

rt_uint32_t get_26infraredProbe_offset(void)//26��Զ�̺���̽ͷ,���㴬��ƫ����.
{
    count = 0;
    sum = 0 ;

    port_E = (0XFFFF)^readPort_E();
    port_F = (0XFFFF)^readPort_F();

    frontProbe = (port_F & 0x1FFF) + ((port_E & 0x1FFF) << 13);
    if (frontProbe == 0)
    {
        return 0;
    }
    for (index = 1; index <= 26; index++)
    {
        if (frontProbe & 0X00000001)
        {
            sum = sum + index;
            count++;
        }
        frontProbe >>= 1;
    }
    return sum / count;
}

rt_uint32_t get_9infraredProbe_offset(void)//9�����̺���̽ͷ,���㴬��ƫ����.�˺������ȼ�Ӧ������get_26infraredProbe_offset.
{
    count = 0;
    sum = 0 ;

    port_D = readPort_D();

    closeProbe = (port_D & 0x01FF);
    if(closeProbe&0x000001C0)
    {
        if(((closeProbe&0x000001C0)==0x000001C0)||((closeProbe&0x000001C0)==0x00000080))//3λ����̽ͷ
        {
            return 0x0000FFFF;//����ȫF ˵�����Ӧ�õ�Ϊ�м�ֵ.
        }
        else if(closeProbe&0x00000040)
        {
            return 0x80000010;//����16 ���λΪ1.
        }
        else if(closeProbe&0x00000100)
        {
            return 0x8000000b;//����11.���λΪ1.
        }
    }
    else if(closeProbe&0x0000003F)//6λ����̽ͷ
    {
        closeProbe = closeProbe&0x003F;
        for(index = 1; index <= 6; index++)
        {
            if(closeProbe & 0x00000001)
            {
                sum = sum + index;
                count++;
            }
            closeProbe >>= 1;
        }
        return (sum / count)|0xC0000000;//����(1-6),����λΪ1.
    }
    return 0;
}
