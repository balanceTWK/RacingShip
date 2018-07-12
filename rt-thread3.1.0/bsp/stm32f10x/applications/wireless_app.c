#include <rthw.h>
#include <rtthread.h>

#include "wireless_app.h"

/* ���ڽ����¼���־ */
#define WIRELESS_RX_EVENT (1 << 0)

/* �¼����ƿ� */
static struct rt_event wirelessevent;
/* �����豸��� */
static rt_device_t wireless_device = RT_NULL;

/* �ص����� */
static rt_err_t wireless_intput(rt_device_t dev, rt_size_t size)
{
    /* �����¼� */
    rt_event_send(&wirelessevent, WIRELESS_RX_EVENT);

    return RT_EOK;
}

rt_uint8_t wireless_getchar(void)
{
    rt_uint32_t e;
    rt_uint8_t ch;

    /* ��ȡ1�ֽ����� */
    while (rt_device_read(wireless_device, 0, &ch, 1) != 1)
    {
        /* �����¼� */
        rt_event_recv(&wirelessevent, WIRELESS_RX_EVENT, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
    }

    return ch;
}
void wireless_putchar(const rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(wireless_device, 0, &c, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);
}

void wireless_putstring(const rt_uint8_t *s)
{
    while (*s)
    {
        wireless_putchar(*s++);
    }
}

rt_err_t wireless_open(const char *name)
{
    rt_err_t res;

    /* ����ϵͳ�еĴ����豸 */
    wireless_device = rt_device_find(name);
    /* ���ҵ��豸����� */
    if (wireless_device != RT_NULL)
    {

        res = rt_device_set_rx_indicate(wireless_device, wireless_intput);
        /* ��鷵��ֵ */
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n", name, res);
            return -RT_ERROR;
        }

        /* ���豸���Կɶ�д���жϷ�ʽ */
        res = rt_device_open(wireless_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
        /* ��鷵��ֵ */
        if (res != RT_EOK)
        {
            rt_kprintf("open %s device error.%d\n", name, res);
            return -RT_ERROR;
        }

    }
    else
    {
        rt_kprintf("can't find %s device.\n", name);
        return -RT_ERROR;
    }

    /* ��ʼ���¼����� */
    rt_event_init(&wirelessevent, "wireless", RT_IPC_FLAG_FIFO);

    return RT_EOK;
}



