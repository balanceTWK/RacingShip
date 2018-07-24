#include <rthw.h>
#include <rtthread.h>
#include "master_app.h"
#include "wireless_app.h"
#include "control_app.h"

void master_thread_entry(void *parameter)
{
    /* 打开串口 */
    if (wireless_open("uart3") != RT_EOK)
    {
        rt_kprintf("uart3 open error.\n");
    }
    wireless_putstring("\r\n\r\nwireless is working.\r\n");
    wireless_putstring("\r\nApplication Author : layiketang.");
    wireless_putstring("\r\nPCBLAYOUT   Author : xiaoluobo.");
    wireless_putstring("\r\n           College : DHKXJSXY.\r\n\r\n");
    rt_thread_delay(rt_tick_from_millisecond(50));
    control_app_init();
}

int master_init(void)
{
    rt_thread_t tid;
    
    /* 创建master线程 */
    tid = rt_thread_create("master",
                           master_thread_entry,
                           RT_NULL,
                           256,
                           5,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(master_init);
