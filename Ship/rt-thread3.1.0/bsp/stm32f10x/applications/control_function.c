#include <rthw.h>
#include <rtthread.h>
#include "inv_mpu.h"
#include "motor.h"
#include "read_io_port.h"
#include "control_function.h"

static rt_uint32_t frontProbe = 0,closeProbe = 0, sum = 0;
static rt_uint16_t port_D = 0, port_E = 0, port_F = 0;

static rt_uint8_t index, count;

rt_uint32_t get_26infraredProbe_offset(void)//26个远程红外探头,计算船体偏移量.
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

rt_uint32_t get_9infraredProbe_offset(void)//9个近程红外探头,计算船体偏移量.此函数优先级应当大于get_26infraredProbe_offset.
{
    count = 0;
    sum = 0 ;

    port_D = readPort_D();

    closeProbe = (port_D & 0x01FF);
    if(closeProbe&0x000001C0)
    {
        if(((closeProbe&0x000001C0)==0x000001C0)||((closeProbe&0x000001C0)==0x00000080))//3位红外探头
        {
            return 0x0000FFFF;//返回全F 说明舵机应该调为中间值.
        }
        else if(closeProbe&0x00000040)
        {
            return 0x80000010;//返回16 最高位为1.
        }
        else if(closeProbe&0x00000100)
        {
            return 0x8000000b;//返回11.最高位为1.
        }
    }
    else if(closeProbe&0x0000003F)//6位红外探头
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
        return (sum / count)|0xC0000000;//返回(1-6),高两位为1.
    }
    return 0;
}
