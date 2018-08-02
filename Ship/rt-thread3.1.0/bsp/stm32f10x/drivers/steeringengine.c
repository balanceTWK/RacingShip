#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "pwm.h"
#include "steeringengine.h"
#include "beep.h"

#define DebugBeep beep(50,3);
#define DebugPrintf(x) rt_kprintf(x)

void steeringEngine_1(rt_uint16_t compare)
{
    if((compare>=500)&&(compare<=1000))
    {
        pwm(1,compare);
    }
    else
    {
        DebugPrintf("\nThe range of the parameter is between 250-1250 .(steeringEngine_1)\n");
        DebugBeep
    }
}

void steeringEngine_2(rt_uint16_t compare)
{
    if((compare>=250)&&(compare<=1250))
    {
        pwm(2,compare);
    }
    else
    {
        DebugPrintf("\nThe range of the parameter is between 250-1250 .(steeringEngine_2)\n");
        DebugBeep
    }
}

void steeringEngine_3(rt_uint16_t compare)
{
    if((compare>=250)&&(compare<=1250))
    {
        pwm(3,compare);
    }
    else
    {
        DebugPrintf("\nThe range of the parameter is between 250-1250 .(steeringEngine_3)\n");
        DebugBeep
    }
}

void steeringEngine_4(rt_uint16_t compare)
{
    if((compare>=250)&&(compare<=1250))
    {
        pwm(4,compare);
    }
    else
    {
        DebugPrintf("\nThe range of the parameter is between 250-1250 .(steeringEngine_4)\n");
        DebugBeep
    }
}