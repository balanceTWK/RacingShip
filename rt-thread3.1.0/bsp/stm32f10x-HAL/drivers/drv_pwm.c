#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>

#define PWM_CHANNEL_MAX     (8) /* 0-7 */

struct rt_qemu_pwm
{
    struct rt_device_pwm parent;

    rt_uint32_t period[PWM_CHANNEL_MAX];
    rt_uint32_t pulse[PWM_CHANNEL_MAX];
};


static struct rt_qemu_pwm _qemu_pwm_device;


static rt_err_t set(struct rt_device_pwm *device, struct rt_pwm_configuration *configuration)
{
    rt_err_t result = RT_EOK;
    struct rt_qemu_pwm *qemu_pwm_device = (struct rt_qemu_pwm *)device;

    if (configuration->channel > (PWM_CHANNEL_MAX - 1))
    {
        result = -RT_EIO;
        goto _exit;
    }

    rt_kprintf("drv_pwm.c set channel: %d, period: %d, pulse: %d\n", configuration->channel, configuration->period, configuration->pulse);

    qemu_pwm_device->period[configuration->channel] = configuration->period;
    qemu_pwm_device->pulse[configuration->channel] = configuration->pulse;

_exit:
    return result;
}

static rt_err_t get(struct rt_device_pwm *device, struct rt_pwm_configuration *configuration)
{
    rt_err_t result = RT_EOK;
    struct rt_qemu_pwm *qemu_pwm_device = (struct rt_qemu_pwm *)device;
	
    if (configuration->channel > (PWM_CHANNEL_MAX - 1))
    {
        result = -RT_EIO;
        goto _exit;
    }
	
	configuration->period = qemu_pwm_device->period[configuration->channel];
	configuration->pulse = qemu_pwm_device->pulse[configuration->channel];
	
_exit:
    return result;
}

static rt_err_t control(struct rt_device_pwm *device, int cmd, void *arg)
{
    rt_err_t result = RT_EOK;

    rt_kprintf("drv_pwm.c control cmd: %d. \n", cmd);

    if (cmd == PWM_CMD_ENABLE)
    {
        rt_kprintf("PWM_CMD_ENABLE\n");
    }
    else if (cmd == PWM_CMD_SET)
    {
        rt_kprintf("PWM_CMD_SET\n");
        result = set(device, (struct rt_pwm_configuration *)arg);
    }
    else if (cmd == PWM_CMD_GET)
    {
        rt_kprintf("PWM_CMD_GET\n");
        result = get(device, (struct rt_pwm_configuration *)arg);
    }

    return result;
}

static const struct rt_pwm_ops pwm_ops =
{
    control,
};


int rt_hw_pwm_init(void)
{
    int ret = RT_EOK;

    /* add pwm initial. */

    ret = rt_device_pwm_register(&_qemu_pwm_device.parent, "pwm", &pwm_ops, RT_NULL);

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_pwm_init);
