#include <rthw.h>
#include <rtthread.h>
#include "inv_mpu.h"
#include "motor.h"
#include "read_io_port.h"
#include "control_function.h"

static rt_uint32_t frontProbe=0 ,sum=0;
static rt_uint16_t port_D=0,port_E=0,port_F=0;

static rt_uint8_t index,count;

rt_uint32_t get_infraredProbe_offset(void)//26个红外探头,计算船体偏移量.
{
    count = 0;
    sum = 0 ;
    
    port_E=readPort_E();
    port_F=readPort_F();
    
    frontProbe=(port_F&0x1FFF)+((port_E&0x1FFF)<<13);
    if(frontProbe==0)
    {
        return 0;
    }
    for(index=1;index<=26;index++)
    {
        if(!(frontProbe&0X00000001))
        {
            sum=sum+index;
            count++;
        }
        frontProbe>>=1;
    }
    return sum/count;
}
