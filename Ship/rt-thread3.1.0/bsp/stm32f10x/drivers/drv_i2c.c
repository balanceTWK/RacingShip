#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "drv_i2c.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...)   rt_kprintf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

/*user can change this*/
#define I2CBUS_NAME  "i2c1"

/*user should change this to adapt specific board*/
#define I2C1_SCL_PIN                 GPIO_Pin_8
#define I2C1_SDA_PIN                 GPIO_Pin_9
#define I2C1_PORT                    GPIOB

static void stm32f1_i2c_gpio_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_PORT, &GPIO_InitStructure);

    GPIO_SetBits(I2C1_PORT, I2C1_SCL_PIN);
    GPIO_SetBits(I2C1_PORT, I2C1_SDA_PIN);
}

static void stm32f1_set_sda(void *data, rt_int32_t state)
{
    if (state)
    {
        I2C1_PORT->BSRR = I2C1_SDA_PIN;
    }
    else
    {
        I2C1_PORT->BSRR = (uint32_t)I2C1_SDA_PIN << 16U;
    }
}

static void stm32f1_set_scl(void *data, rt_int32_t state)
{
    if (state)
    {
        I2C1_PORT->BSRR = I2C1_SCL_PIN;
    }
    else
    {
        I2C1_PORT->BSRR = (uint32_t)I2C1_SCL_PIN << 16U;
    }
}

static rt_int32_t  stm32f1_get_sda(void *data)
{
    if ((I2C1_PORT->IDR & I2C1_SDA_PIN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static rt_int32_t  stm32f1_get_scl(void *data)
{
    if ((I2C1_PORT->IDR & I2C1_SCL_PIN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void stm32f1_udelay(rt_uint32_t us)
{
    rt_uint32_t delta;

    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
    delta = SysTick->VAL;
    while (delta - SysTick->VAL < us);
}

static const struct rt_i2c_bit_ops stm32f1_bit_ops =
{
    RT_NULL,
    stm32f1_set_sda,
    stm32f1_set_scl,
    stm32f1_get_sda,
    stm32f1_get_scl,
    stm32f1_udelay,
    1,
    100
};

int hw_i2c_init(void)
{
    static struct rt_i2c_bus_device i2c2_bus;

    stm32f1_i2c_gpio_init();
    rt_memset((void *)&i2c2_bus, 0, sizeof(struct rt_i2c_bus_device));
    i2c2_bus.priv = (void *)&stm32f1_bit_ops;
    rt_i2c_bit_add_bus(&i2c2_bus, I2CBUS_NAME);

    return RT_EOK;
}
INIT_BOARD_EXPORT(hw_i2c_init);
/* end of i2c driver */

#define DRV_I2C_DEBUG
#if defined(RT_USING_FINSH) && defined(DRV_I2C_DEBUG)
#include <finsh.h>

static rt_device_t _i2c_find(const char *name)
{
    rt_device_t dev;

    dev = rt_device_find(name);
    if (!dev)
    {
        rt_kprintf("search device failed: %s\n", name);
        return RT_NULL;
    }
    if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed: %s\n", name);
        return RT_NULL;
    }
    rt_kprintf("open i2c bus: %s\n", name);
    return dev;
}

static void _search_i2c_device(rt_device_t dev, uint8_t cmd)
{
    int count = 0;
    struct rt_i2c_msg msgs[2];
    uint8_t buf = 0;

    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &cmd;
    msgs[0].len   = sizeof(cmd);

    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = &buf;
    msgs[1].len   = 1;

    for (int i = 0; i <= 0x7f; i++)
    {
        int len;
        msgs[0].addr  = i;
        msgs[1].addr  = i;
        len = rt_i2c_transfer((struct rt_i2c_bus_device *)dev, msgs, 2);
        if (len == 2)
        {
            count++;
            rt_kprintf("add:%02X transfer success, id: %02X\n", i, buf);
        }
    }
    rt_kprintf("i2c device: %d\n", count);
}

static int i2c_test(const char *name, uint8_t cmd)
{
    rt_device_t dev = _i2c_find(name);

    if (dev == RT_NULL)
    {
        rt_kprintf("search i2c device faild\n");
        return -1;
    }
    _search_i2c_device(dev, cmd);
    rt_device_close(dev);
    return 0;
}
FINSH_FUNCTION_EXPORT(i2c_test, e.g: i2c_test("i2c1", 0xA3));

#endif
